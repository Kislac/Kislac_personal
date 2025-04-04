# Dev Environment setup 

## Automatic Version update
1. Install Python (add to the path)
2. Install  following dependencies in windows command line: 
```
pip install platformio
pip install scons
```
3. Open new terminal in platfrom io within your project. Check is the pio working or not: ```pio --version```
4. Create versions.txt in ```data/version.txt``` folder and give an initial number, like 0
5. Upload versions.txt to MCU:
```pio run --target uploadfs```
6. Create python scripts which is increment the number per builds. Place at the project root folder.
```python
import os
from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

def increment_version(source, target, env):
    version_file = os.path.join(env['PROJECT_DIR'], 'data', 'version.txt')
    
    # Read the current version number
    with open(version_file, 'r') as file:
        version = int(file.read().strip())
    
    # Increment the version number
    version += 1
    
    # Write the new version number back to the file
    with open(version_file, 'w') as file:
        file.write(str(version))
    
    print(f"Version incremented to {version}")
    
    # Upload the SPIFFS filesystem
    env.Execute("pio run --target uploadfs")

env.AddPreAction("upload", increment_version)
```
7.  platformio.ini file: 
```extra_scripts = pre:increment_version.py```
8. Add the code to your project setup() part:
```c++
// Read the build number from version.txt
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

    File versionFile = SPIFFS.open("/version.txt", "r");
    if (!versionFile) {
      Serial.println("Failed to open version file");
      return;
    }

    int build_number = versionFile.parseInt();
    Serial.printf("Build_number: %d \n", build_number);
    versionFile.close();

    // Update the ui_VerValue with the code version
    char version[16];
    sprintf(version, "1.0.%d", build_number);
    lv_label_set_text(ui_VerValue, version);
```