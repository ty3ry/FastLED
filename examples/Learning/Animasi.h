#ifndef ANIMASI_H
#define ANIMASI_H

#include <FastLED.h>

#define NUM_LEDS    24


class Animasi
{
private:
    CRGB leds[NUM_LEDS];

public:
    Animasi();
    ~Animasi();
    
    void animasi_1(void);
    void Basic(void);
    void RainbowCycle(void);
    void Pulse(int ms);
    void PulseShow(void);
};




#endif /** end of ANIMASI_H */