#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24           // Harus genap agar simetris
#define BRIGHTNESS  150
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  EVERY_N_MILLISECONDS(20) {
    gHue++;  // Putar warna perlahan
  }

  // Hitung "nafas" atau efek membesar-menutup
  uint8_t wave = beatsin8(4, 0, 255);  // Frekuensi 4 = pelan & lembut

  // Hitung titik tengah
  int center = NUM_LEDS / 2;

  // Hapus LED lama
  fadeToBlackBy(leds, NUM_LEDS, 40);

  // Buat pola dari tengah ke luar
  for (int i = 0; i < center; i++) {
    uint8_t brightness = scale8(wave, 255 - (i * 255 / center)); // Efek kedalaman
    leds[center + i] += CHSV(gHue, 255, brightness);
    leds[center - i - 1] += CHSV(gHue, 255, brightness);
  }

  FastLED.show();
  delay(10);
}
