#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24         // Sesuaikan dengan jumlah LED pada ring kamu
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
uint8_t heat[NUM_LEDS];
uint8_t rotationOffset = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  generateFireEffect();      // 🔥 Buat efek api
  rotateLEDs(2);             // 🔄 Rotasi satu langkah searah jarum jam
  FastLED.show();
  delay(30);
}

// 🔥 Fungsi utama simulasi api
void generateFireEffect() {
  // 1. Pendinginan
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, 3));
  }

  // 2. Sebarkan panas ke LED tetangga (blur manual)
  for (int i = 0; i < NUM_LEDS; i++) {
    int prev = (i - 1 + NUM_LEDS) % NUM_LEDS;
    int next = (i + 1) % NUM_LEDS;
    heat[i] = (heat[i] + heat[prev] + heat[next]) / 3;
  }

  // 3. Tambah percikan api
  if (random8() < 150) {
    int pos = random8(NUM_LEDS);
    heat[pos] = qadd8(heat[pos], random8(160, 255));
  }

  // 4. Konversi suhu ke warna
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = HeatColor(heat[i]);  // Merah → oranye → kuning
  }
}

// 🔁 Rotasi array `leds[]`
void rotateLEDs(uint8_t steps) {
  // Simpel: geser isi array ke kanan sebanyak `steps`
  for (uint8_t s = 0; s < steps; s++) {
    CRGB last = leds[NUM_LEDS - 1];
    for (int i = NUM_LEDS - 1; i > 0; i--) {
      leds[i] = leds[i - 1];
    }
    leds[0] = last;
  }
}
