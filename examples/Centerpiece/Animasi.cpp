#include <Arduino.h>
#include "Animasi.h"
#include "Timeout.h"

// use for pacifica effect
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


/**
 * @brief   Constructor
 */
Animasi::Animasi(void)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
}

/**
 * @brief   Destructor
 */
Animasi::~Animasi(void) {
    
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
    static uint8_t sat = 255;

    EVERY_N_MILLISECONDS(5) {
        for(int i = 0; i < NUM_LEDS; i += 1) {
            leds[i] = CHSV(mHue + (i * 20), sat, 100);
        }
        mHue++;
    }
}


void Animasi::confetti(void) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(mHue + random8(64), 200, 255);
}

void Animasi::sinelon(void) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(mHue, 255, 192);
}

void Animasi::bpm(void) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        leds[i] =
            ColorFromPalette(palette, mHue + (i * 2), beat - mHue + (i * 10));
    }
}

void Animasi::juggle(void) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, NUM_LEDS, 20);
    uint8_t dothue = 0;
    for (uint16_t i = 0; i < 8; i++) {
        leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
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


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  90

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

void Animasi::Fire2012(void)
{
    EVERY_N_MILLISECONDS(30){
        static bool gReverseDirection = false;

        // Array of temperature readings at each simulation cell
        static uint8_t heat[NUM_LEDS];

        // Step 1.  Cool down every cell a little
        for( int i = 0; i < NUM_LEDS; i++) {
            heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
        }

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for( int k= NUM_LEDS - 1; k >= 2; k--) {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
        }

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if( random8() < SPARKING ) {
            int y = random8(7);
            heat[y] = qadd8( heat[y], random8(160,255) );
        }

        // Step 4.  Map from heat cells to LED colors
        for( int j = 0; j < NUM_LEDS; j++) {
            CRGB color = HeatColor( heat[j]);
            int pixelnumber;
            if( gReverseDirection ) {
            pixelnumber = (NUM_LEDS-1) - j;
            } else {
            pixelnumber = j;
            }
            leds[pixelnumber] = color;
        }
        // FastLED.delay(1000 / 60);
    }
}

void Animasi::BreathingTunnel(void)
{
    EVERY_N_MILLISECONDS(20) {
        mHue++;  // Putar warna perlahan
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
        leds[center + i] += CHSV(mHue, 255, brightness);
        leds[center - i - 1] += CHSV(mHue, 255, brightness);
    }
}

void Animasi::generateFireEffect()
{
    static uint8_t heat[NUM_LEDS];

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

void Animasi::rotateLEDs(uint8_t steps)
{
    for (uint8_t s = 0; s < steps; s++) {
        CRGB last = leds[NUM_LEDS - 1];
        for (int i = NUM_LEDS - 1; i > 0; i--) {
        leds[i] = leds[i - 1];
        }
        leds[0] = last;
    }
}

void Animasi::FireFlame(void)
{
    EVERY_N_MILLISECONDS(30){
    generateFireEffect();
    rotateLEDs(2);
    }
}

void Animasi::LarsonScan(void)
{
    EVERY_N_MILLISECONDS(30) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i].fadeToBlackBy(80);
        }

        // Nyalakan LED di posisi saat ini
        leds[mPos] = CRGB::Blue;

        // Ubah posisi cahaya
        mPos += mDirection;
        if (mPos == NUM_LEDS - 1 || mPos == 0) {
            mDirection = -mDirection; // Balik arah di ujung
        }
    }
}

void Animasi::BidirectionalPulse(void)
{
    EVERY_N_MILLISECONDS(30){
        fadeToBlackBy(leds, NUM_LEDS, 25);  // trail

        int center = NUM_LEDS / 2;
        int offset = mFrame;

        // Kalkulasi posisi LED kiri dan kanan
        int left = mOutward ? center - offset : offset;
        int right = mOutward ? center + offset - 1 : NUM_LEDS - 1 - offset;

        // Nyalakan LED kiri dan kanan
        if (left >= 0 && left < NUM_LEDS) {
            leds[left] = CHSV(mHue, 255, 255);  // warna biru kehijauan
        }
        if (right >= 0 && right < NUM_LEDS) {
            leds[right] = CHSV(mHue, 255, 255);
        }

        mFrame++;

        if (mOutward && (center + mFrame >= NUM_LEDS || center - mFrame < 0)) {
            mFrame = 0;
            mOutward = false;  // ubah arah ke dalam
        } else if (!mOutward && (mFrame >= center)) {
            mFrame = 0;
            mOutward = true;   // ubah arah ke luar lagi
        }

        mHue = (mHue + 1) % 255;
    }
}

void Animasi::pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
    uint16_t ci = cistart;
    uint16_t waveangle = ioff;
    uint16_t wavescale_half = (wavescale / 2) + 20;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        waveangle += 250;
        uint16_t s16 = sin16(waveangle) + 32768;
        uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
        ci += cs;
        uint16_t sindex16 = sin16(ci) + 32768;
        uint8_t sindex8 = scale16(sindex16, 240);
        CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
        leds[i] += c;
    }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void Animasi::pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void Animasi::pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

void Animasi::Pacifica(void)
{
    // Increment the four "color index start" counters, one for each wave layer.
    // Each is incremented at a different speed, and the speeds vary over time.
    static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
    static uint32_t sLastms = 0;
    uint32_t ms = GET_MILLIS();
    uint32_t deltams = ms - sLastms;
    sLastms = ms;
    uint16_t speedfactor1 = beatsin16(3, 179, 269);
    uint16_t speedfactor2 = beatsin16(4, 179, 269);
    uint32_t deltams1 = (deltams * speedfactor1) / 256;
    uint32_t deltams2 = (deltams * speedfactor2) / 256;
    uint32_t deltams21 = (deltams1 + deltams2) / 2;
    sCIStart1 += (deltams1 * beatsin88(1011,10,13));
    sCIStart2 -= (deltams21 * beatsin88(777,8,11));
    sCIStart3 -= (deltams1 * beatsin88(501,5,7));
    sCIStart4 -= (deltams2 * beatsin88(257,4,6));

    // Clear out the LED array to a dim background blue-green
    fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

    // Render each of four layers, with different scales and speeds, that vary over time
    pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
    pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
    pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
    pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

    // Add brighter 'whitecaps' where the waves lines up more
    pacifica_add_whitecaps();

    // Deepen the blues and greens a bit
    pacifica_deepen_colors();
}

void Animasi::RunPattern(uint8_t option)
{
    switch (option) {
        default: break;
        case MODE_RAINBOW:
            RainbowCycle();
            break;
        case MODE_BLUR:
            Blur();
            break;
        case MODE_CONFETTI:
            confetti();
            break;
        case MODE_SINELON:
            sinelon();
            break;
        case MODE_BPM:
            bpm();
            break;
        case MODE_JUGGLE:
            juggle();
            break;
        case MODE_FIRE2012:
            Fire2012();
            // FireFlame();
            break;
        case MODE_BREATHING_TUNNEL:
            BreathingTunnel();
            break;
        case MODE_LARSON_SCAN:
            LarsonScan();
            break;
        case MODE_BIDIRECTIONAL_PULSE:
            BidirectionalPulse();
            break;
        case MODE_PACIFICA:
            Pacifica();
            break;
    }

    FastLED.show();
}