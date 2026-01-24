#pragma once

#include <Arduino.h>

#include "config.h"

typedef enum {
    LCD_CHAR_ARROW_UP,
    LCD_CHAR_ARROW_DOWN,
} LcdCharArrowsScreenId;

typedef enum {
    LCD_ENCODER_STATE_00,
    LCD_ENCODER_STATE_01,
    LCD_ENCODER_STATE_11,
    LCD_ENCODER_STATE_10,
    LCD_CHAR_CHECKMARK,
} LcdCharEncoderScreenId;

extern const uint8_t arrowUp[8];
extern const uint8_t arrowDown[8];
extern const uint8_t encoderState_00[8];
extern const uint8_t encoderState_01[8];
extern const uint8_t encoderState_11[8];
extern const uint8_t encoderState_10[8];
extern const uint8_t checkmark[8];
