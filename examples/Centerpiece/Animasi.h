#ifndef ANIMASI_H
#define ANIMASI_H

#include <FastLED.h>

#define LED_PIN     5

#define NUM_LEDS    30
#define FRAME_PER_SECOND    60
#define BRIGHTNESS  150

enum AnimationMode {
    MODE_RAINBOW = 0,
    MODE_BLUR,
    MODE_CONFETTI,
    MODE_SINELON,
    MODE_BPM,
    MODE_JUGGLE,
    MODE_FIRE2012,
    MODE_BREATHING_TUNNEL,
    MODE_LARSON_SCAN,
    MODE_BIDIRECTIONAL_PULSE,
    MODE_PACIFICA,
    MODE_MAX,
};

enum ColorIndex {
    COLOR_RED = 0,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_WHITE,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_PURPLE,
    COLOR_ORANGE,
    COLOR_MAX,
};

enum AnimMode {
    LED_MODE_ANIM = 0,
    LED_MODE_COLOR_STATIC,
    LED_MODE_MAX,
};

class Animasi
{
private:
    CRGB leds[NUM_LEDS];
    uint8_t mHue;
    // for larsonScan effect
    int mPos = 0;           // Posisi cahaya
    int mDirection = 1;     // 1 = ke kanan, -1 = ke kiri

    // bidrirectional pulse
    int mFrame = 0;
    bool mOutward = true;  

    void generateFireEffect();
    void rotateLEDs(uint8_t steps);

    // helper function for pacifica effect
    void pacifica_deepen_colors();
    void pacifica_add_whitecaps();
    void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);

public:
    Animasi();
    ~Animasi();
    
    void RainbowCycle(void);
    void Pulse(int ms);
    void PulseShow(void);
    void RunPattern(uint8_t option);
    void Blur(void);
    void confetti(void);
    void sinelon(void);
    void bpm(void);
    void juggle(void);
    void Fire2012(void);
    void BreathingTunnel(void);
    void FireFlame(void);
    void LarsonScan(void);
    void BidirectionalPulse(void);
    void Pacifica(void);
    void StaticColor(unsigned int color_index);
    void ProgressTrail(void);

    void Show(void);
};




#endif /** end of ANIMASI_H */