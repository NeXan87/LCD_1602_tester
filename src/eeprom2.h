#pragma once

#include <Arduino.h>

struct Settings {
    uint32_t magic;        // сигнатура для проверки валидности
    int backlightPercent;  // 0–100
    // Добавьте другие поля по мере необходимости
};

void eepromInit();
void eepromSaveSettings();
void eepromLoadSettings();
int eepromGetBacklightPercent();
void eepromSetBacklightPercent(int percent);
