#include <FastLED.h>

#define LED_PIN     5 
#define NUM_LEDS    24
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int pos = 0;           // Posisi cahaya
int direction = 1;     // 1 = ke kanan, -1 = ke kiri

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();     // Matikan semua LED di awal
}

void loop() {
  // Redupkan semua LED perlahan untuk efek trail
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(80);  // Nilai lebih tinggi = trail lebih pendek
  }

  // Nyalakan LED di posisi saat ini
  leds[pos] = CRGB::Blue;

  // Tampilkan ke strip
  FastLED.show();
  delay(30);  // Semakin kecil = semakin cepat

  // Ubah posisi cahaya
  pos += direction;
  if (pos == NUM_LEDS - 1 || pos == 0) {
    direction = -direction;  // Balik arah di ujung
  }
}
