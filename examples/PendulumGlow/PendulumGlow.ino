#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    24       // Ubah sesuai jumlah LED kamu
#define BRIGHTNESS  150
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int pos = 0;
int direction = 1; // 1: kanan, -1: kiri

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  fadeToBlackBy(leds, NUM_LEDS, 30);  // Membuat jejak memudar

  leds[pos] = CHSV(160, 255, 255);    // Warna biru kehijauan

  FastLED.show();
  delay(100);

  pos += direction;
  if (pos == NUM_LEDS - 1 || pos == 0) {
    direction = -direction; // Balik arah saat ujung
  }
}
