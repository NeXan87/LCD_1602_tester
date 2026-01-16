#pragma once

#include <Arduino.h>

#include "config.h"

void lcdInit();
void lcdClear();
void lcdSetCursor(uint8_t col, uint8_t row);
void lcdPrint(const char* str);
void lcdPrintInt(long value);
void lcdPrintf(const char* format, ...);
