#include "drivers/lcd-custom-chars.h"

const uint8_t arrowUp[8] = {
    B00000,
    B00100,
    B01110,
    B10101,
    B00100,
    B00100,
    B00100,
    B00000};

const uint8_t arrowDown[8] = {
    B00000,
    B00100,
    B00100,
    B00100,
    B10101,
    B01110,
    B00100,
    B00000};

const uint8_t checkmark[8] = {
    B00000,
    B00001,
    B00011,
    B10110,
    B11100,
    B01000,
    B00000,
    B00000};

// Состояние: A=0, B=0 → оба сигнала LOW
const uint8_t encoderState_00[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,  // нижняя линия — оба LOW
    B00000};

// Состояние: A=0, B=1
const uint8_t encoderState_01[8] = {
    B00000,
    B00111,
    B00100,
    B00100,
    B00100,
    B00100,
    B11100,
    B00000};

// Состояние: A=1, B=1 → оба HIGH
const uint8_t encoderState_11[8] = {
    B00000,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B00000};

// Состояние: A=1, B=0
const uint8_t encoderState_10[8] = {
    B00000,
    B11100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00111,
    B00000};

const uint8_t encoderState_BOTH_11[8] = {
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};
