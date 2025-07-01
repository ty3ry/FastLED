#ifndef ANIMASI_H
#define ANIMASI_H

#include <FastLED.h>

#define LED_PIN     5

#define NUM_LEDS    24
#define FRAME_PER_SECOND    60


class Animasi
{
private:
    CRGB leds[NUM_LEDS];
    uint8_t hue;
public:
    Animasi();
    ~Animasi();
    
    void animasi_1(void);
    void Basic(void);
    void RainbowCycle(void);
    void Pulse(int ms);
    void PulseShow(void);
    void Testing(uint8_t option);
    void Blur(void);
    void confetti(void);
    void sinelon(void);
    void bpm(void);
    void juggle(void);
};




#endif /** end of ANIMASI_H */