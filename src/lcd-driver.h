#pragma once

#include <Arduino.h>

#include "config.h"

void initArrowsLCD();
void initLCD();
void clearLCD();
void setCursorLCD(uint8_t col, uint8_t row);
void writeCharLCD(uint8_t c);
void printLCD(const char* str);
void printIntLCD(long value);
void printfLCD(const char* format, ...);
