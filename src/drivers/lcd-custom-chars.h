#pragma once

#include <Arduino.h>

#include "config.h"

typedef enum {
    LCD_CHAR_ARROW_UP,
    LCD_CHAR_ARROW_DOWN,
} LcdCharAboutScreenId;

extern const uint8_t arrowUp[8];
extern const uint8_t arrowDown[8];
