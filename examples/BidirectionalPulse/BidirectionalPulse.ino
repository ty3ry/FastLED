#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24           // Jumlah LED (genap lebih baik)
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int frame = 0;
bool outward = true;             // arah: true = keluar, false = masuk

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
}

void loop() {
  fadeToBlackBy(leds, NUM_LEDS, 25);  // trail

  int center = NUM_LEDS / 2;
  int offset = frame;

  // Kalkulasi posisi LED kiri dan kanan
  int left = outward ? center - offset : offset;
  int right = outward ? center + offset - 1 : NUM_LEDS - 1 - offset;

  // Nyalakan LED kiri dan kanan
  if (left >= 0 && left < NUM_LEDS) {
    leds[left] = CHSV(160, 255, 255);  // warna biru kehijauan
  }
  if (right >= 0 && right < NUM_LEDS) {
    leds[right] = CHSV(160, 255, 255);
  }

  FastLED.show();
  delay(30);

  frame++;

  if (outward && (center + frame >= NUM_LEDS || center - frame < 0)) {
    frame = 0;
    outward = false;  // ubah arah ke dalam
  } else if (!outward && (frame >= center)) {
    frame = 0;
    outward = true;   // ubah arah ke luar lagi
  }
}
