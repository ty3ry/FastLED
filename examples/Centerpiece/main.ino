#include <FastLED.h>
#include <Arduino.h>
#include "Animasi.h"
#include "Key.h"
#include "Message.h"
#include "stm32g0xx_hal.h"

static Animasi anime;
static EventContext msgSend;
static Key key;
unsigned long int option = MODE_PACIFICA;
unsigned int colorIndex = 0;
unsigned char led_mode = 0;

#define FLASH_USER_START_ADDR   0x08007C00  // alamat akhir Flash STM32G030J6 (32KB total)
#define DATA_32                 ((uint32_t)0x12345678)

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
    colorIndex = readFlash(FLASH_USER_START_ADDR);
    if (colorIndex > COLOR_MAX) colorIndex = 0;
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
            if (led_mode == LED_MODE_ANIM) {
                option = (option + 1) % MODE_MAX;
            } else {
                colorIndex = (colorIndex + 1) % COLOR_MAX;
                writeFlash( FLASH_USER_START_ADDR, colorIndex );
            }
            break;

        case MSG_SELECT_MODE:
            /**
             * toggle to selec anim mode
             * 0: animation
             * 1: static color
             */
            led_mode = !led_mode;
            break;
    }

    if (led_mode == 0) {
        /** do select animation */
        anime.RunPattern(option);
    } else {
        anime.StaticColor(colorIndex);
    }

    /** led show */
    anime.Show();

}