#include <Adafruit_NeoPixel.h>

#define LED_PIN    7      // WS2812B adatláb
#define LED_COUNT  1      // Csak 1 LED van a modulon

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // LED kikapcsolása
}

void loop() {
  // Piros
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
  delay(500);

  // Zöld
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  delay(500);

  // Kék
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
  delay(500);

  // Kikapcs
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
  delay(500);
}