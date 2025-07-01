#include <FastLED.h>
#include <Arduino.h>
#include "Animasi.h"
#include "Key.h"
#include "Message.h"

static Animasi anime;
static EventContext msgSend;
static Key key;
unsigned long int option = MODE_PACIFICA;

void setup()
{
    /** reserved */
}

void loop()
{
    msgSend.eventId = MSG_NONE;

    EVERY_N_MILLISECONDS(20) {
        msgSend.eventId = key.Scan();
    }

    switch(msgSend.eventId) {
        default:
            break;

        case MSG_MODE:
            option = (option + 1) % MODE_MAX;
            break;
    }

    /** do select animation */
    anime.RunPattern(option);

}