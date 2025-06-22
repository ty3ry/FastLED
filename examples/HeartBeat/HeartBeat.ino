#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

enum HeartbeatState {
  IDLE,
  BEAT_1_ON,
  BEAT_1_OFF,
  BEAT_2_ON,
  BEAT_2_OFF,
  REST
};

HeartbeatState state = IDLE;
unsigned long lastStateChange = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  unsigned long now = millis();

  switch (state) {
    case IDLE:
      if (now - lastStateChange > 1000) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        state = BEAT_1_ON;
        lastStateChange = now;
      }
      break;

    case BEAT_1_ON:
      if (now - lastStateChange > 100) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        state = BEAT_1_OFF;
        lastStateChange = now;
      }
      break;

    case BEAT_1_OFF:
      if (now - lastStateChange > 100) {
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        state = BEAT_2_ON;
        lastStateChange = now;
      }
      break;

    case BEAT_2_ON:
      if (now - lastStateChange > 80) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        state = BEAT_2_OFF;
        lastStateChange = now;
      }
      break;

    case BEAT_2_OFF:
      if (now - lastStateChange > 800) {
        state = IDLE;
        lastStateChange = now;
      }
      break;

    case REST:
      // (Tidak digunakan dalam versi ini — siklus kembali ke IDLE)
      break;
  }
}
