#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24          // Ganti sesuai jumlah LED
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

uint8_t heartbeatPhase = 0;     // 0 = diam, 1 = detak pertama, 2 = detak kedua
unsigned long lastBeatTime = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  unsigned long currentTime = millis();

  // Fase detak jantung
  switch (heartbeatPhase) {
    case 0: // Istirahat
      if (currentTime - lastBeatTime > 1000) {  // Setiap 1 detik mulai detak
        heartbeatPhase = 1;
        lastBeatTime = currentTime;
      }
      break;

    case 1: // Detak pertama (kuat)
      showPulse(CRGB::Red, 255);
      heartbeatPhase = 2;
      lastBeatTime = currentTime;
      break;

    case 2: // Detak kedua (lemah)
      if (currentTime - lastBeatTime > 120) {
        showPulse(CRGB::Red, 150);
        heartbeatPhase = 0;
        lastBeatTime = currentTime;
      }
      break;
  }

  fadeToBlackBy(leds, NUM_LEDS, 20);
  FastLED.show();
  delay(20);
}

void showPulse(CRGB color, uint8_t brightness) {
  int center = NUM_LEDS / 2;
  for (int i = 0; i <= center; i++) {
    int indexL = center - i;
    int indexR = center + i;
    uint8_t b = scale8(brightness, 255 - i * (255 / center));

    if (indexL >= 0) leds[indexL] += color.nscale8(b);
    if (indexR < NUM_LEDS) leds[indexR] += color.nscale8(b);
  }
}
