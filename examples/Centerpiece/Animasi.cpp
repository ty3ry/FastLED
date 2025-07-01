#include <Arduino.h>
#include "Animasi.h"
#include "Timeout.h"

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
    static uint8_t sat = 255;

    EVERY_N_MILLISECONDS(5) {
        for(int i = 0; i < NUM_LEDS; i += 1) {
            leds[i] = CHSV(hue + (i * 20), sat, 100);
        }
        
        //FastLED.show();
        hue++;
    }
    //delay(5);
}

void Animasi::Testing(uint8_t option)
{
    /*static Timeout tmrBlink;
    static uint8_t led_state = 0;

    if (tmrBlink.IsTimeOut()) {
        tmrBlink.TimeOutSet(500);
        if (led_state ^= 0x01) {
            leds[0] = CRGB::Blue;
        } else {
            leds[0] = CRGB::Black;
        }
    }*/

    switch (option) {
        default: break;
        case 0:
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            leds[0] = CRGB::Red;
            break;
        case 1:
            leds[0] = CRGB::Green;
            break;
        case 2: 
            leds[0] = CRGB::Blue;
            break;
        case 3:
            RainbowCycle();
            break;
        case 4:
            Blur();
            break;
        case 5:
            confetti();
            break;
        case 6:
            sinelon();
            break;
        case 7:
            bpm();
            break;
        case 8:
            juggle();
            break;
    }

    FastLED.show();
}

void Animasi::confetti(void) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, mNumLeds, 10);
    int pos = random16(mNumLeds);
    leds[pos] += CHSV(hue + random8(64), 200, 255);
}

void Animasi::sinelon(void) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, mNumLeds, 20);
    int pos = beatsin16(13, 0, mNumLeds - 1);
    leds[pos] += CHSV(hue, 255, 192);
}

void Animasi::bpm(void) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (uint16_t i = 0; i < mNumLeds; i++) {
        leds[i] =
            ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
    }
}

void Animasi::juggle(void) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, mNumLeds, 20);
    uint8_t dothue = 0;
    for (uint16_t i = 0; i < 8; i++) {
        leds[beatsin16(i + 7, 0, mNumLeds - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}


void Animasi::Blur(void)
{
    static uint8_t pos = 0;
    static bool toggle = false;

    EVERY_N_MILLISECONDS(20) {
        // Add a bright pixel that moves
        leds[pos] = CHSV(pos * 2, 255, 255);
        // Blur the entire strip
        blur1d(leds, NUM_LEDS, 172);
        fadeToBlackBy(leds, NUM_LEDS, 16);
        // Move the position of the dot
        if (toggle) {
            pos = (pos + 1) % NUM_LEDS;
        }
        toggle = !toggle;
    }
}