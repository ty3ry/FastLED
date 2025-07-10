#include <FastLED.h>
#include <Arduino.h>
#include "Animasi.h"
#include "Key.h"
#include "Message.h"
#include "stm32g0xx_hal.h"

#define FLASH_USER_START_ADDR   0x08007C00

static Animasi anime;
static EventContext msgSend;
static Key key;
unsigned long int option = MODE_PACIFICA;
unsigned int colorIndex = 0;
uint8_t brightness = 50;
unsigned char led_mode = 0;

union NVM_Data
{
    struct
    {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    };
    uint32_t word;
};

NVM_Data nvmData;

void writeFlash(uint32_t address, uint32_t data) {
  HAL_FLASH_Unlock();
  
  // Hapus page
  FLASH_EraseInitTypeDef eraseInitStruct;
  uint32_t pageError;
  eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseInitStruct.Page = (address - 0x08000000) / FLASH_PAGE_SIZE;
  eraseInitStruct.NbPages = 1;

  HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);

  // Tulis data
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data);

  HAL_FLASH_Lock();
}

uint32_t readFlash(uint32_t address) {
  return *(__IO uint32_t*)address;
}

void setup()
{
    // load from NVM
    nvmData.word = readFlash(FLASH_USER_START_ADDR);

    nvmData.b2 = 0xaa;
    nvmData.b3 = 0x55;

    colorIndex = nvmData.b0;
    brightness = nvmData.b1;

    if (brightness <= 0 || brightness > 100 ) brightness = BRIGHTNESS_PERCENT;
    if (colorIndex > COLOR_MAX) colorIndex = 0;

    // set FastLED brightness
    anime.SetBrightness(brightness);
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
            switch( led_mode ) {
            case LED_MODE_ANIM:
                option = (option + 1) % MODE_MAX;
                break;

            case LED_MODE_COLOR_STATIC:
                colorIndex = (colorIndex + 1) % COLOR_MAX;
                nvmData.b0 = (uint8_t) colorIndex;
                writeFlash( FLASH_USER_START_ADDR, nvmData.word );
                break;

            case LED_MODE_SET_BRIGHTNESS:
                brightness = (brightness + 10) % 100;
                nvmData.b1 = brightness;
                writeFlash( FLASH_USER_START_ADDR, nvmData.word );

                if (brightness == 0) brightness = 1;
                break;
            }

            break;

        case MSG_SELECT_MODE:
            /**
             * toggle to selec anim mode
             * 0: animation
             * 1: static color
             * 2: set brightness
             */
            led_mode = (led_mode + 1) % LED_MODE_MAX;
            break;
    }

    if (led_mode == LED_MODE_ANIM) {
        anime.RunPattern( option );
    } else if (led_mode == LED_MODE_COLOR_STATIC) {
        anime.StaticColor( colorIndex );
    } else if (led_mode == LED_MODE_SET_BRIGHTNESS) {
        anime.SetBrightShow( brightness );
    } else {
    }

    /** led show */
    anime.Show();

}