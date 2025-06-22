#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    24       // Jumlah LED pada ring
#define BRIGHTNESS  180
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

uint8_t heat[NUM_LEDS];      // Simulasi suhu seperti efek fire
uint8_t hueOffset = 0;       // Perputaran warna

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // 1. Kurangi "panas" untuk simulasi pendinginan
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, 4));
  }

  // 2. Sebarkan panas antara tetangga
  for (int i = 0; i < NUM_LEDS; i++) {
    int next = (i + 1) % NUM_LEDS;
    int prev = (i - 1 + NUM_LEDS) % NUM_LEDS;
    heat[i] = (heat[i] + heat[next] + heat[prev]) / 3;
  }

  // 3. Tambahkan "percikan" api berwarna
  if (random8() < 100) {
    int pos = random8(NUM_LEDS);
    heat[pos] = qadd8(heat[pos], random8(180, 255));
  }

  // 4. Konversi suhu ke warna pelangi
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t thisHue = hueOffset + scale8(heat[i], 192); // warna bervariasi sesuai suhu
    leds[i] = CHSV(thisHue, 255, heat[i]);
  }

  // 5. Putar seluruh array untuk efek rotasi
  rotateLEDs(leds, NUM_LEDS, 1);

  hueOffset++;  // Rotasi warna
  FastLED.show();
  delay(30);
}

// 🔁 Fungsi rotasi array LED ke kanan
void rotateLEDs(CRGB* arr, uint8_t len, uint8_t steps) {
  while (steps--) {
    CRGB last = arr[len - 1];
    for (int i = len - 1; i > 0; i--) {
      arr[i] = arr[i - 1];
    }
    arr[0] = last;
  }
}
