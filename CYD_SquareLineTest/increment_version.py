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