#include <FastLED.h>
#include <SoftwareSerial.h>


#define LED_PIN         24 
#define BUTTON_PIN      PF2

#define PERIODE     500

uint32_t start = 0;
uint8_t state = 0;

void setup() {
    Serial.begin(115200);

    Serial.println("-- TEST PIN --");
    FastPin<LED_PIN>::setOutput();
    FastPin<BUTTON_PIN>::setInput();

    start = millis();

    //pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {

    if (millis() > start + PERIODE) {
        start = millis();

        if (state ^= 1) {
            FastPin<LED_PIN>::hi();
        } else {
            FastPin<LED_PIN>::lo();
        }
    }

    if (/*FastPin<BUTTON_PIN>::loval()*/ digitalRead(BUTTON_PIN)) {
        Serial.println("Btn Pressed!");
    }
}
