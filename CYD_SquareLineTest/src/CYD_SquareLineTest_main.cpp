#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <FS.h>     // Include the SPIFFS library for Version handling
#include <SPIFFS.h> // Include the SPIFFS library for Version handling
//Include dependecies for WAV file playback from SD card
#include <SD.h>
#include <SPI.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

////ESP now
//#include <esp_now.h>
//#include <WiFi.h>
//// Adatstruktúra
//typedef struct struct_message {
//    char a[32];
//    int32_t rssi;
//} struct_message;
//
//struct_message incomingData;

//#include <NewPing.h>
//#define TRIGGER_PIN  16  // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN     16  // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#include <Wire.h>
#include <Adafruit_AHTX0.h>
// Define I2C pins
// Define I2C pins for AHT10 (IO21 for SDA, IO22 for SCL)
#define AHT10_SDA 21
#define AHT10_SCL 22

// Define I2C pins for VL53L1X (IO21 for SDA, IO22 for SCL)
#define VL53L1X_SDA 21
#define VL53L1X_SCL 22

// Create a TwoWire object for I2C_NUM_1
TwoWire I2C_AHT10 = TwoWire(1);  // Specify the I2C peripheral (1 for I2C_NUM_1)

Adafruit_AHTX0 aht = Adafruit_AHTX0();  // Create the sensor object
float temperature = 0;
float humidity = 0;


TwoWire I2C_VL53L1X = TwoWire(1);  // Specify the I2C peripheral (1 for I2C_NUM_1)
#include <VL53L1X.h>
VL53L1X sensor;// = VL53L1X();

// Define arrays to store temperature and humidity data
float temp_data[10] = {0};
float hum_data[10] = {0};
int data_index = 0;
// Define the array for the chart series
lv_coord_t ui_ChartTempHum_series_1_array[10] = {0};
lv_coord_t ui_ChartTempHum_series_2_array[10] = {0};
//Ultrasound init
//#include <NewPing.h>
//#define TRIGGER_PIN  1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE 450 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// SD kártya csatlakoztatási lábak
#define SD_CS_PIN 5
#define SD_MOSI_PIN 23
#define SD_MISO_PIN 19
#define SD_SCK_PIN 18

AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioOutputI2S *out;

// Hangerő (0.0 - 1.0)
float WavVolume = 0.5;
bool isWavEnabled = false;  // Csipogás alapértelmezetten kikapcsolva


//Set the screen brightness to 50% initially
#define SCREEN_INITIAL_BRIGHTNESS 50
ulong next_millis;
auto lv_last_tick = millis();
char text_buffer[32];

// A csipogó GPIO láb
const int buzzerPin = 26;
int Freq = 1500;


int64_t now = 0;

// Mario főcímdalának dallama (hangok frekvenciája Hz-ben)
int melody[] = {
  659, 659, 0, 659, 0, 523, 659, 784, 0, 392, 523, 392, 330, 440, 494, 466, 440
};

// Hangok időtartama (milli-szekundumban)
int noteDurations[] = {
  150, 150, 150, 150, 150, 150, 150, 150, 300, 300, 300, 300, 150, 150, 150, 150, 300
};

// Hangerő (0-255)
int volume = 5;
bool isEnabled = false;  // Csipogás alapértelmezetten kikapcsolva

//void OnAddOneClicked(lv_event_t *e)
//{
//    static uint32_t cnt = 0;
//    cnt++;
//    lv_label_set_text_fmt(ui_lblCountValue, "%u", cnt);
//}
//
//void OnRotateClicked(lv_event_t *e)
//{
//    auto disp = lv_disp_get_default();
//    auto rotation = (lv_display_rotation_t)((lv_disp_get_rotation(disp) + 1) % (LV_DISPLAY_ROTATION_270 + 1));
//    lv_display_set_rotation(disp, rotation);
//}
bool USSEnabled = false;

// Callback függvény az adat fogadásához
//void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//    struct_message myData;
//    memcpy(&myData, incomingData, sizeof(myData));
//    Serial.print("Bytes received: ");
//    Serial.println(len);
//    Serial.print("Message: ");
//    Serial.println(myData.a);
//    Serial.print("RSSI: ");
//    Serial.println(myData.rssi);
//    sprintf(text_buffer, "%s", myData.a);
//    lv_label_set_text(ui_lblMsg, text_buffer);
//    sprintf(text_buffer, "%d %%", myData.rssi);
//    lv_label_set_text(ui_lblRSSI, text_buffer);
//}

void funcSetBackLight(lv_event_t * e)
{
  if (!(lv_obj_get_state(ui_switchManualAutomatic) & LV_STATE_CHECKED)) {
    // Visszaolvassuk a csúszka értékét
    int slider_value = lv_slider_get_value(ui_LigthSensorSlider);
    float lcd_set_backlight = (float)slider_value / 100;
    Serial.printf("slider_value: %d | lcd_set_backlight: %.2f\n", slider_value, lcd_set_backlight);
    smartdisplay_lcd_set_backlight(max(lcd_set_backlight, 0.01f));
  }
	
}

void funcManualAutomatic(lv_event_t * e)
{
  int switchManualAutomatic_value = lv_obj_get_state(ui_switchManualAutomatic) & LV_STATE_CHECKED ? 1 : 0;
  Serial.printf("switchManualAutomatic_value: %d\n", switchManualAutomatic_value);
	if (switchManualAutomatic_value == 1) {
		lv_obj_set_style_text_color(ui_lblManual, lv_color_hex(0x000000), LV_PART_MAIN);
		lv_obj_set_style_text_decor(ui_lblManual, LV_TEXT_DECOR_NONE, LV_PART_MAIN);
		lv_obj_set_style_text_color(ui_lblAutomatic, lv_color_hex(0xFF0000), LV_PART_MAIN);
		lv_obj_set_style_text_decor(ui_lblAutomatic, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN);
	} else {
		lv_obj_set_style_text_color(ui_lblManual, lv_color_hex(0xFF0000), LV_PART_MAIN);
		lv_obj_set_style_text_decor(ui_lblManual, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN);
		lv_obj_set_style_text_color(ui_lblAutomatic, lv_color_hex(0x000000), LV_PART_MAIN);
		lv_obj_set_style_text_decor(ui_lblAutomatic, LV_TEXT_DECOR_NONE, LV_PART_MAIN);
	}
}

void initializeBuildInfo()
{
  
  // Get and print the build date and time
  Serial.printf("Build Date: %s, Build Time: %s\n", __DATE__, __TIME__);

  // Update the ui_BuildDateValue with the build date and time
  lv_label_set_text_fmt(ui_BuildDateValue, "%s %s", __DATE__, __TIME__);
  
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
  sprintf(version, "v1.0.%d", build_number);
  lv_label_set_text(ui_VerValue, version);
}

void PlayMusic() {
  if (isEnabled) {
    // Marió Zene lejátszás
    static int noteIndex = 0;
    static ulong noteStartTime = 0;

    if (millis() - noteStartTime >= noteDurations[noteIndex]) {
      noteStartTime = millis();
      if (noteIndex < sizeof(melody) / sizeof(melody[0])) {
        if (melody[noteIndex] == 0) {
          ledcWriteTone(0, 0);  // Csend
        } else {
          ledcWriteTone(0, melody[noteIndex]);
          ledcWrite(0, volume);  // Hangerő beállítása
        }
        noteIndex++;
      } else {
        noteIndex = 0;  // Újraindítja a dallamot
      }
    }
  }
}

void funcPlayFreq(lv_event_t * e)
{
	int SwPlayFreq_value = lv_obj_get_state(ui_SwPlayFreq) & LV_STATE_CHECKED ? 1 : 0;
  
	if (SwPlayFreq_value == 1) {

    ledcWriteTone(0, Freq);   // Hang kiadása a megfelelő frekvencián
    ledcWrite(0, volume);          // Hangerő beállítása
	} else {
    ledcWriteTone(0, 0);   // Hang némítása
	}

}

void funcPlayMusic(lv_event_t * e)
{
	int SwPlayMusic_value = lv_obj_get_state(ui_SwPlayMusic) & LV_STATE_CHECKED ? 1 : 0;
	if (SwPlayMusic_value == 1) {
		isEnabled = true;
	} else {
		isEnabled = false;
    ledcWriteTone(0, 0);   // Hang némítása
	}
  Serial.printf("isEnabled: %d\n", isEnabled);

}

void func_FreqSlider(lv_event_t * e)
{
  if (!(lv_obj_get_state(ui_sldFreq) & LV_STATE_CHECKED)) {
  // Visszaolvassuk a csúszka értékét
  int sldFreq_value = lv_slider_get_value(ui_sldFreq);
  Freq = sldFreq_value;
  Serial.printf("ui_sldFreq: %d\n", sldFreq_value);
  if (lv_obj_get_state(ui_SwPlayFreq) & LV_STATE_CHECKED) {
    ledcWriteTone(0, Freq);
  }
  }
}

void funcVolume(lv_event_t * e)
{
	if (!(lv_obj_get_state(ui_sldVolume) & LV_STATE_CHECKED)) {
    // Visszaolvassuk a csúszka értékét
    int sldVolume_value = lv_slider_get_value(ui_sldVolume);
    volume = map(sldVolume_value, 0, 100, 0, 255);
    WavVolume = (float)sldVolume_value / 100.0;
    out->SetGain(WavVolume);  // Hangerő beállítása
    Serial.printf("sldVolume_value: %d | volume: %d | WavVolume:%.2f\n", sldVolume_value, volume, WavVolume);
    if ((lv_obj_get_state(ui_SwPlayFreq) & LV_STATE_CHECKED) || (lv_obj_get_state(ui_SwPlayMusic) & LV_STATE_CHECKED)) {
      ledcWrite(0, volume);          // Hangerő beállítása
    }
  }
}

void funcPlayWavMusic(lv_event_t * e)
{
	int SwPlayWavMusic_value = lv_obj_get_state(ui_SwPlayWavMusic) & LV_STATE_CHECKED ? 1 : 0;
	if (SwPlayWavMusic_value == 1) {
		isWavEnabled = true;
    file = new AudioFileSourceSD("/numb.wav");
    wav = new AudioGeneratorWAV();
    wav->begin(file, out);
	} else {
		isWavEnabled = false;
    wav->stop();
    ledcWriteTone(0, 0);   // Hang némítása
    Serial.println("Lejátszás vége.");
	}
  Serial.printf("isWavEnabled: %d\n", isWavEnabled);

}

void PlayWavMusic()
{
  if (isWavEnabled) {
    if (wav->isRunning()) {
      wav->loop();
    } else {
      wav->stop();
      Serial.println("Lejátszás vége.");
      //isEnabled = false;  // Lejátszás vége után kikapcsoljuk a csipogást
    }
    
  }
}




void UltrasonicRead() {
  static ulong lastUSSReadTime = 0;
  ulong currentUSSTime = millis();

  if (currentUSSTime - lastUSSReadTime >= 1 && USSEnabled) {
    lastUSSReadTime = currentUSSTime;
      sensor.read();
      //int distance = sonar.ping_cm();
      //if (distance == 0) {
      //  sprintf(text_buffer, "Out of range");
      //} else {
      //  
      //}
      sprintf(text_buffer, "%d mm", sensor.ranging_data.range_mm);
      lv_label_set_text(ui_lblUSSValue, text_buffer);
      // Írd ki a távolságot a soros monitorra
      //Serial.printf("\rTávolság: %d cm", distance);

      

//      Serial.print("range: ");
//      Serial.print(sensor.ranging_data.range_mm);
//      Serial.print("\tstatus: ");
//      Serial.print(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
//      Serial.print("\tpeak signal: ");
//      Serial.print(sensor.ranging_data.peak_signal_count_rate_MCPS);
//      Serial.print("\tambient: ");
//      Serial.print(sensor.ranging_data.ambient_count_rate_MCPS);
//
//      Serial.println();

  }
}

void funcReset(lv_event_t * e)
{
  Serial.printf("\n\n============================\nPerforming software reset...\n============================\n\n");
 

  delay(1000); // Optional delay to allow the message to be sent
  esp_restart();
}

void ReadAHT10()
{

  static ulong lastReadTime = 0;
  ulong currentTime = millis();
  
  if (currentTime - lastReadTime >= 500) {
    lastReadTime = currentTime;

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);  // Get the temperature and humidity event data
    float TempValue = temp.temperature;
    float HumValue = humidity.relative_humidity;
    // Store the data in the arrays
    temp_data[data_index] = TempValue;
    hum_data[data_index] = HumValue;
    // Shift the data to the left
    for (int i = 0; i < 9; i++) {
      temp_data[i] = temp_data[i + 1];
      hum_data[i] = hum_data[i + 1];
    }
    // Add the new data to the end
    temp_data[9] = TempValue;
    hum_data[9] = HumValue;

    //Serial.printf("\rHőmérséklet: %.2f °C | Páratartalom: %.2f%% ", temp.temperature, humidity.relative_humidity);
    sprintf(text_buffer, "%.2f °C", TempValue);
    lv_label_set_text(ui_lblTemp, text_buffer);
    sprintf(text_buffer, "%.2f %%", HumValue);
    lv_label_set_text(ui_lblHum, text_buffer);

    // Update the chart
    //for (int i = 0; i < 10; i++) {
    //  ui_ChartTempHum_Yaxis1[i]->points[i] = temp_data[i];
    //  ui_ChartTempHum_Yaxis2[i]->points[i] = hum_data[i];

    //------------
    //lv_chart_series_t * ui_ChartTempHum_series_1 = lv_chart_add_series(ui_ChartTempHum, lv_color_hex(0xFA0000),
    //                                                                   LV_CHART_AXIS_PRIMARY_Y);
    //static lv_coord_t ui_ChartTempHum_series_1_array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    //lv_chart_set_ext_y_array(ui_ChartTempHum, ui_ChartTempHum_series_1, ui_ChartTempHum_series_1_array);
    //----------
    
    //}
    //lv_chart_series_t * ui_ChartTempHum_series_1 = lv_chart_add_series(ui_ChartTempHum, lv_palette_main(LV_PALETTE_RED),LV_CHART_AXIS_PRIMARY_Y);
    //lv_chart_series_t * ser1 = lv_chart_add_series(ui_ChartTempHum, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    //lv_chart_series_t * ui_ChartTempHum_series_2 = lv_chart_add_series(ui_ChartTempHum, lv_palette_main(LV_PALETTE_BLUE),LV_CHART_AXIS_SECONDARY_Y);                                                                    
    //lv_chart_series_t * ser2 = lv_chart_add_series(ui_ChartTempHum, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);
    //lv_coord_t ui_ChartTempHum_series_1_array[] = { 0, 10, 20, 40, 1, 10, 40, 20, 10, 0, 0 };
    //static lv_chart_series_t * ui_ChartTempHum_series_1;
    //static lv_chart_series_t * ui_ChartTempHum_series_2;
    // Update the chart series array with temp_data
    //Serial.printf("\rPáratartalom: ");
    for (int i = 0; i < 10; i++) {
      ui_ChartTempHum_series_1_array[i] = temp_data[i];
      ui_ChartTempHum_series_2_array[i] = hum_data[i];
      //Serial.printf("[%.2f]",  hum_data[i]);
    }
    //lv_chart_set_ext_y_array(ui_ChartTempHum, ui_ChartTempHum_series_1, ui_ChartTempHum_series_1_array);
    //lv_chart_set_ext_y_array(ui_ChartTempHum, ui_ChartTempHum_series_2, ui_ChartTempHum_series_2_array);
    
    //lv_chart_set_next_value(ui_ChartTempHum, ser1, TempValue);
    //lv_chart_set_next_value(ui_ChartTempHum, ser2, HumValue);
    //ser1->y_points[0] = 10;
    //ser1->y_points[1] = 20;
    //ser1->y_points[2] = 30;
    //lv_chart_set_ext_y_array(ui_ChartTempHum, ui_ChartTempHum_series_1, temp_data_int);
    //lv_chart_set_ext_y_array(ui_ChartTempHum, ui_ChartTempHum_series_2, hum_data_int);
    //lv_chart_refresh(ui_ChartTempHum);  // Refresh the chart


    /*Create a chart*/
    //lv_obj_t * chart;
    //chart = lv_chart_create(ui_Screen4);
    ////ui_ChartTempHum = lv_chart_create(ui_Screen4);
    //lv_obj_set_size(chart, 200, 150);
    //lv_obj_center(chart);
    //lv_obj_set_y(chart, 0);
    //lv_obj_set_align(chart, LV_ALIGN_CENTER);
    //lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    /*Add two data series*/
    ////lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    ////lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);
////
    /////*Set the next points on 'ser1'*/
    ////
    ////int32_t temp_data_int[10];
    ////int32_t hum_data_int[10];
    ////for (int i = 0; i < 10; i++) {
    ////    temp_data_int[i] = static_cast<int32_t>(temp_data[i]);
    ////    lv_chart_set_next_value(chart, ser1, temp_data_int[i]);
    ////    hum_data_int[i] = static_cast<int32_t>(hum_data[i]);
    ////    lv_chart_set_next_value(chart, ser2, hum_data_int[i] );
    ////}

    //lv_chart_set_next_value(chart, ser1, 10);
    //lv_chart_set_next_value(chart, ser1, 40);
    //lv_chart_set_next_value(chart, ser1, 10);
    //lv_chart_set_next_value(chart, ser1, 30);
    //lv_chart_set_next_value(chart, ser1, 10);
    //lv_chart_set_next_value(chart, ser1, 10);
    //lv_chart_set_next_value(chart, ser1, 10);
    //lv_chart_set_next_value(chart, ser1, 30);
    //lv_chart_set_next_value(chart, ser1, 70);
    //lv_chart_set_next_value(chart, ser1, 90);

    //lv_chart_set_next_value(chart, ser2, 60);
    //lv_chart_set_next_value(chart, ser2, 30);
    //lv_chart_set_next_value(chart, ser2, 20);
    //lv_chart_set_next_value(chart, ser2, 100);

     /*Directly set points on 'ser2'*/
    //ser2->y_points[0] = 90;
    //ser2->y_points[1] = 70;
    //ser2->y_points[2] = 65;
    //ser2->y_points[3] = 65;
    //ser2->y_points[4] = 65;
    //ser2->y_points[5] = 65;
    //ser2->y_points[6] = 65;
    //ser2->y_points[7] = 65;
    //ser2->y_points[8] = 65;
    //ser2->y_points[9] = 65;

    //lv_chart_refresh(chart); /*Required after direct set*/
  }
}

void funcUSSenable(lv_event_t * e)
{
	int ui_swUSS_value = lv_obj_get_state(ui_swUSS) & LV_STATE_CHECKED ? 1 : 0;
	if (ui_swUSS_value == 1) {
		USSEnabled = true;
	} else {
		USSEnabled = false;
    //sprintf(text_buffer, "%d cm");
    lv_label_set_text(ui_lblUSSValue, "Disabled!");
	}
}


void readVL53L1X() {
  sensor.read();

  Serial.print("range: ");
  Serial.print(sensor.ranging_data.range_mm);
  Serial.print("\tstatus: ");
  Serial.print(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
  Serial.print("\tpeak signal: ");
  Serial.print(sensor.ranging_data.peak_signal_count_rate_MCPS);
  Serial.print("\tambient: ");
  Serial.print(sensor.ranging_data.ambient_count_rate_MCPS);

  Serial.println();
}

void runtimecalc(){
  int64_t total_seconds = now / 1000;
  int years = total_seconds / (365 * 24 * 3600);
  total_seconds %= (365 * 24 * 3600);
  int days = total_seconds / (24 * 3600);
  total_seconds %= (24 * 3600);
  int hours = total_seconds / 3600;
  total_seconds %= 3600;
  int minutes = total_seconds / 60;
  float seconds = total_seconds % 60 + (now % 1000) / 1000.0;

  if (lv_tabview_get_tab_act(ui_TabView2) == 4) {
      // Update the audio tab
      sprintf(text_buffer, "%02d year %03d day", years, days);
      lv_label_set_text(ui_lblRuntimeYDValue, text_buffer);
      sprintf(text_buffer, "%02d hour %02d min %02.1f sec", hours, minutes, seconds);
      lv_label_set_text(ui_lblRuntimeHMSValue, text_buffer);
  }
}


void AutoBacklight(){
  #ifdef BOARD_HAS_CDS
        auto cdr = analogReadMilliVolts(CDS);
        sprintf(text_buffer, "%d", cdr);
        if (lv_tabview_get_tab_act(ui_TabView2) == 3) {
            lv_label_set_text(ui_lblLigthSensorValue, text_buffer);
        }
        
        if (lv_obj_get_state(ui_switchManualAutomatic) & LV_STATE_CHECKED) {
            float adaptive_brightness = smartdisplay_lcd_adaptive_brightness_cds();
            smartdisplay_lcd_set_backlight(adaptive_brightness);
            if (lv_tabview_get_tab_act(ui_TabView2) == 3) {
              int slider_value = (int)(adaptive_brightness * 100);
              lv_slider_set_value(ui_LigthSensorSlider, slider_value, LV_ANIM_OFF);
              sprintf(text_buffer, "%d%%", slider_value);
              lv_label_set_text(ui_lblLigthSensorSliderValue, text_buffer);
            }
        }
      #endif
}

void AutoLedBlinking(){
#ifdef BOARD_HAS_RGB_LED
        auto const rgb = (now / 2000) % 8;
        smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
#endif
}

void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif

    

  Serial.begin(115200);


// Initialize I2C bus for VL53L1X
    I2C_VL53L1X.begin(VL53L1X_SDA, VL53L1X_SCL);
    I2C_VL53L1X.setClock(400000); // use 400 kHz I2C

    sensor.setBus(&I2C_VL53L1X);  // Set the I2C bus for VL53L1X
    sensor.setTimeout(500);
    if (!sensor.init()) {
        Serial.println("Failed to detect and initialize sensor!");
    } else {
        Serial.println("Sensor initialized successfully!");
    }

    sensor.setDistanceMode(VL53L1X::Long);
    sensor.setMeasurementTimingBudget(100000);
    sensor.startContinuous(100);  


//  Wire.begin();
//  Wire.setClock(400000); // use 400 kHz I2C
//
//  sensor.setTimeout(500);
//  if (!sensor.init())
//  {
//    Serial.println("Failed to detect and initialize sensor!");
//    while (1);
//  }
//
//  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
//  // You can change these settings to adjust the performance of the sensor, but
//  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
//  // medium and long distance modes. See the VL53L1X datasheet for more
//  // information on range and timing limits.
//  sensor.setDistanceMode(VL53L1X::Long);
//  sensor.setMeasurementTimingBudget(50000);
//
//  // Start continuous readings at a rate of one measurement every 50 ms (the
//  // inter-measurement period). This period should be at least as long as the
//  // timing budget.
//  sensor.startContinuous(50);

  // Initialize I2C bus for AHT10
  //I2C_AHT10.begin(AHT10_SDA, AHT10_SCL, 100000);  // 100kHz I2C speed
  //I2C_AHT10.begin(AHT10_SDA, AHT10_SCL);
  // Initialize the AHT10 sensor on the custom I2C bus
  if (!aht.begin(&I2C_VL53L1X)) {
    Serial.println("AHT10 initialization failed.");
  } else {
    Serial.println("AHT10 sensor initialized.");
  }
     
    //// WiFi inicializálása
    //WiFi.mode(WIFI_STA);
    //WiFi.disconnect();
    //
    //// ESP-NOW inicializálása
    //if (esp_now_init() != ESP_OK) {
    //    Serial.println("Error initializing ESP-NOW");
    //    return;
    //}
    //
    //// Callback függvény beállítása
    //esp_now_register_recv_cb(OnDataRecv);
    

    Serial.setDebugOutput(false);
    //log_i("Board: %s", board);
    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());

    smartdisplay_init();

    __attribute__((unused)) auto disp = lv_disp_get_default();
     lv_disp_set_rotation(disp, LV_DISPLAY_ROTATION_90);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();

    //// To use third party libraries, enable the define in lv_conf.h: #define LV_USE_QRCODE 1
    //auto ui_qrcode = lv_qrcode_create(ui_scrMain);
    //lv_qrcode_set_size(ui_qrcode, 100);
    //lv_qrcode_set_dark_color(ui_qrcode, lv_color_black());
    //lv_qrcode_set_light_color(ui_qrcode, lv_color_white());
    //const char *qr_data = "https://github.com/rzeldent/esp32-smartdisplay";
    //lv_qrcode_update(ui_qrcode, qr_data, strlen(qr_data));
    //lv_obj_center(ui_qrcode);

    // Set the screen brightness to 50% initially
    smartdisplay_lcd_set_backlight(max(((float)(SCREEN_INITIAL_BRIGHTNESS) / 100), 0.01f));
    // Update the ui_LigthSensorSlider to reflect the 50% brightness
    lv_slider_set_value(ui_LigthSensorSlider, SCREEN_INITIAL_BRIGHTNESS, LV_ANIM_OFF);
    sprintf(text_buffer, "%d%%", SCREEN_INITIAL_BRIGHTNESS);
    lv_label_set_text(ui_lblLigthSensorSliderValue, text_buffer);

    // Set the switch to manual mode initially
    lv_obj_add_state(ui_switchManualAutomatic, LV_STATE_DEFAULT);
    funcManualAutomatic(nullptr);    // Call the funcManualAutomatic function after setting the switch

    initializeBuildInfo();

    // LEDC csatorna beállítása (a LEDC timer az ESP32-n egy PWM vezérlőt jelent)
    ledcSetup(0, 5000, 8); // 0. csatorna, 5kHz alapfrekvencia, 8 bites felbontás
    // GPIO 26 kimenet hozzárendelése a LEDC-hez
    ledcAttachPin(buzzerPin, 0);

    // SD kártya inicializálása
    if (!SD.begin(SD_CS_PIN)) {
      Serial.println("SD kártya inicializálása sikertelen!");
      return;
    }
    Serial.println("SD kártya inicializálva.");

  // Audio kimenet inicializálása
    out = new AudioOutputI2S(0, 1);  // I2S kimenet inicializálása DAC módra
    out->SetPinout(26, 25, 26);  // GPIO 26-os, 25-os és 22-es lábak beállítása
    out->SetGain(WavVolume);  // Hangerő beállítása
  

  
//  sensor.setTimeout(500);
//  if (!sensor.init())
//  {
//    Serial.println("Failed to detect and initialize sensor!");
//    //while (1);
//  }
//  else
//  {
//    Serial.println("Sensor initialized successfully!");
//  }
//  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
//  // You can change these settings to adjust the performance of the sensor, but
//  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
//  // medium and long distance modes. See the VL53L1X datasheet for more
//  // information on range and timing limits.
//  sensor.setDistanceMode(VL53L1X::Long);
//  sensor.setMeasurementTimingBudget(100000);
//
//  // Start continuous readings at a rate of one measurement every 50 ms (the
//  // inter-measurement period). This period should be at least as long as the
//  // timing budget.
//  sensor.startContinuous(100);

}


void loop()
{
    now = millis();
    if (millis() - next_millis >= 100) {
      next_millis = millis();
      runtimecalc();
      AutoBacklight();
      AutoLedBlinking();
    }
    PlayMusic();
    PlayWavMusic();
    UltrasonicRead();
    ReadAHT10();
    //int ActiveTab = lv_tabview_get_tab_act(ui_TabView2);
    //Serial.printf("\rActive Tab: %d", ActiveTab);
    
    // Update the ticker
    lv_tick_inc(now - lv_last_tick);
    lv_last_tick = now;
    // Update the UI
    lv_timer_handler();
}