#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

enum HeartbeatState {
  IDLE,
  BEAT_1,
  BEAT_2,
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
      if (now - lastStateChange > 500) {  // jeda awal antar detak ganda
        state = BEAT_1;
        lastStateChange = now;
        triggerPulse(CRGB::Red, 255);
      }
      break;

    case BEAT_1:
      if (now - lastStateChange > 600) {  // jeda antar dua detak cepat
        state = BEAT_2;
        lastStateChange = now;
        triggerPulse(CRGB::Red, 200);
      }
      break;

    case BEAT_2:
      if (now - lastStateChange > 100) {
        state = REST;
        lastStateChange = now;
      }
      break;

    case REST:
      if (now - lastStateChange > 600) {  // jeda panjang sebelum ulang
        state = IDLE;
        lastStateChange = now;
      }
      break;
  }

  fadeToBlackBy(leds, NUM_LEDS, 20);
  FastLED.show();
  delay(20);
}

void triggerPulse(CRGB color, uint8_t brightness) {
  int center = NUM_LEDS / 2;
  for (int i = 0; i <= center; i++) {
    int left = center - i;
    int right = center + i;
    uint8_t b = scale8(brightness, 255 - i * (255 / center));

    if (left >= 0) leds[left] += color.nscale8(b);
    if (right < NUM_LEDS) leds[right] += color.nscale8(b);
  }
}
