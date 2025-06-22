#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24
#define BRIGHTNESS  100
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  // Redupkan semua LED sedikit demi sedikit
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(20);
  }

  // Pilih LED acak untuk menyala
  int pos = random(NUM_LEDS);
  leds[pos] += CHSV(random8(), 200, 255);  // Tambahkan warna random

  FastLED.show();
  delay(50);
}
