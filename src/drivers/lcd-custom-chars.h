#pragma once

#include <Arduino.h>

#include "config.h"

typedef enum {
    LCD_CHAR_ARROW_UP,
    LCD_CHAR_ARROW_DOWN,
    LCD_CHAR_CHECKMARK,
    LCD_CHAR_RIGHT_ARROW,
    LCD_CHAR_LEFT_ARROW,
} LcdCharAboutScreenId;

extern const uint8_t arrowUp[8];
extern const uint8_t arrowDown[8];
extern const uint8_t checkmark[8];
extern const uint8_t rightArrow[8];
extern const uint8_t leftArrow[8];
