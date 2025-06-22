#include <FastLED.h>
#include <SoftwareSerial.h>


#define TEST_PIN 12 // Misalnya kamu pakai PB4 dan sudah definisikan _FL_DEFPIN(4, 4, B)

void setup() {
    Serial.begin(115200);

    Serial.println("-- LED Fire2012! --");
    FastPin<TEST_PIN>::setOutput(); // Set pin sebagai output

    // pinMode(12, OUTPUT);
}

void loop() {
    FastPin<TEST_PIN>::hi(); // Nyala
    // digitalWrite(TEST_PIN, 1);
    delay(500);
    FastPin<TEST_PIN>::lo(); // Mati
    // digitalWrite(TEST_PIN, 0);
    delay(500);
}
