#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  Serial.setDebugOutput(false);
    //log_i("Board: %s", board);
    log_i("CPU: %s rev%d, CPU Freq: %d Mhz, %d core(s)", ESP.getChipModel(), ESP.getChipRevision(), getCpuFrequencyMhz(), ESP.getChipCores());
    log_i("Free heap: %d bytes", ESP.getFreeHeap());
    log_i("Free PSRAM: %d bytes", ESP.getPsramSize());
    log_i("SDK version: %s", ESP.getSdkVersion());
    smartdisplay_init();
    __attribute__((unused)) auto disp = lv_disp_get_default();
    //lv_disp_set_rotation(disp, LV_DISPLAY_ROTATION_90);
    lv_disp_set_rotation(disp, LV_DISPLAY_ROTATION_180);
    // lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    ui_init();
}
ulong next_millis;
auto lv_last_tick = millis();
char text_buffer[32];
void loop() {
  int64_t now = millis();


  // Update the ticker
    lv_tick_inc(now - lv_last_tick);
    lv_last_tick = now;
    // Update the UI
    lv_timer_handler();
}

