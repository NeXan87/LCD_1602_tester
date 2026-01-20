#pragma once

#include <Arduino.h>

#include "config.h"

struct Settings {
    uint32_t magic;             // сигнатура для проверки валидности
    int backlightPercent;       // 0–100
    IsEnableId batteryEnabled;  // Включена ли поддержка батареи
    // Добавьте другие поля по мере необходимости
};

void initEeprom();
void saveSettingsEeprom();
void eepromLoadSettings();
int getBacklightPercentEeprom();
void setBacklightPercentEeprom(int percent);
IsEnableId getBatteryEnabledEeprom();
void setBatteryEnabledEeprom(IsEnableId enabled);
