#include "Animasi.h"

#define LED_PIN     12

/**
 * @brief   Constructor
 */
Animasi::Animasi(void)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
}

/**
 * @brief   Destructor
 */
Animasi::~Animasi(void) {
    
}

/** @brief wipe  */
void Animasi::animasi_1(void){
    for(int i = 0; i < NUM_LEDS; i += 1) {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
    }
    FastLED.clear();
}

/**
 * @brief basic function
 */
void Animasi::Basic(void)
{
    static uint32_t time = 0;
    static uint8_t state = 0;

#if 0
    /** memberikan warna pada index led tertentu */
    leds[0] = CRGB::BlueViolet;
    
    /** effect blink pada index led 12 */
    if (millis() > time + 500) {
        time = millis();

        if (state ^= 1) {
            leds[12] = CRGB(255, 0, 0);
        } else {
            leds[12] = CRGB(0, 0, 0);
        }
    }
#endif

#if 1
    EVERY_N_MILLISECONDS(500) {
        if (state ^= 1) {
            fill_solid(leds, NUM_LEDS, CRGB::Black);
        } else {
            fill_rainbow(leds, NUM_LEDS, 100, 60);
        }
    }
#endif

    /** its show time */
    FastLED.show();
}

void Animasi::Pulse(int ms) {
    for(int i = 0; i < 256; i+=5) {
        fill_solid(leds, NUM_LEDS, CRGB(i, 0, 0));
        FastLED.show();
        delay(ms / 100);
    }

    for(int i = 255; i >= 0; i-=5) {
        fill_solid(leds, NUM_LEDS, CRGB(i, 0, 0));
        FastLED.show();
        delay(ms / 100);
    }
}

void Animasi::PulseShow(void)
{
    Pulse(100);
    delay(100);
    Pulse(200);
    delay(600);
}

/** @brief rainbow cycle */
void Animasi::RainbowCycle(void){
    static uint8_t hue = 0;
    static uint8_t sat = 0;
    static uint32_t time;

    for(int i = 0; i < NUM_LEDS; i += 1) {
        // hue, saturation, brightness
        leds[i] = CHSV(hue + (i * 20), sat, 100);
    }

    if (millis() > time + 100) {
        time = millis();
        sat = (sat + 1) % 255;
    }
    
    FastLED.show();
    hue++;
    delay(10);
}