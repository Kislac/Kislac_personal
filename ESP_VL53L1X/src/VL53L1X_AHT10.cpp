#include <Arduino.h>
//
//void setup() {
//  Serial.begin(115200);
//}
//
//void loop() {
//  Serial.println("Hello, World!");
//  delay(1000); // wait for a second
//}


/*
This example takes range measurements with the VL53L1X and displays additional
details (status and signal/ambient rates) for each measurement, which can help
you determine whether the sensor is operating normally and the reported range is
valid. The range is in units of mm, and the rates are in units of MCPS (mega
counts per second).
*/

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;


#include <Wire.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
// Define I2C pins
// Define I2C pins for AHT10 (IO21 for SDA, IO22 for SCL)
#define AHT10_SDA 21
#define AHT10_SCL 22

// Create a TwoWire object for I2C_NUM_1
//TwoWire I2C_AHT10 = TwoWire(1);  // Specify the I2C peripheral (1 for I2C_NUM_1)

//Adafruit_AHTX0 aht = Adafruit_AHTX0();  // Create the sensor object
float temperature = 0;
float humidity = 0;

ulong lastReadTime = 0;

void setup()
{
  //while (!Serial) {}
  Serial.begin(115200);


  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    //while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");



  Wire.begin();
  //Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    //while (1);
  }
  else
  {
    Serial.println("Sensor initialized successfully!");
  }

  // Scan for I2C devices and print their addresses
  Serial.println("Scanning for I2C devices...");
  byte count = 0;
  Wire.begin();
  for (byte i = 1; i < 127; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      if (i < 16) Serial.print("0");
      Serial.print(i, HEX);
      Serial.println(" !");
      count++;
    }
  }
  if (count == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  // Initialize I2C bus for AHT10
  //I2C_AHT10.begin(AHT10_SDA, AHT10_SCL, 100000);  // 100kHz I2C speed
  //I2C_AHT10.begin(AHT10_SDA, AHT10_SCL);
  
  // Initialize the AHT10 sensor on the custom I2C bus
  //if (!aht.begin(&I2C_AHT10)) {
  //  Serial.println("AHT10 initialization failed.");
  //} else {
  //  Serial.println("AHT10 sensor initialized.");
  //}

  

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(100000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(100);
}

void loop()
{
  

  //Serial.print("range: ");
  //Serial.print(sensor.ranging_data.range_mm);
  //Serial.print("\tstatus: ");
  //Serial.print(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
  //Serial.print("\tpeak signal: ");
  //Serial.print(sensor.ranging_data.peak_signal_count_rate_MCPS);
  //Serial.print("\tambient: ");
  //Serial.print(sensor.ranging_data.ambient_count_rate_MCPS);
//
//
  ////Serial.println("Hello, World!");
  ////delay(1000); // wait for a second
  //Serial.println();


  
  ulong currentTime = millis();
  
  if (currentTime - lastReadTime >= 100) {
    lastReadTime = currentTime;
    
    sensor.read();
    int distance = sensor.ranging_data.range_mm;


    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);  // Get the temperature and humidity event data
    float TempValue = temp.temperature;
    float HumValue = humidity.relative_humidity;


    //Serial.printf("\rRange: %dmm | Hőmérséklet: %.2f °C | Páratartalom: %.2f%% ", sensor.ranging_data.range_mm, temp.temperature, humidity.relative_humidity);
    Serial.printf("Range: %dmm | Hőmérséklet: %.2f °C | Páratartalom: %.2f%% \n", distance, TempValue, HumValue);
    //Serial.printf("Range: %dmm | Temp: %.2fC | Hum: %.2f% \n", sensor.ranging_data.range_mm, TempValue, HumValue);
  }
    
    
}