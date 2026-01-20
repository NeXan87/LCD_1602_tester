#pragma once

#include <Arduino.h>

struct Settings {
    uint32_t magic;          // сигнатура для проверки валидности
    int backlightPercent;    // 0–100
    uint8_t batteryEnabled;  // Включена ли поддержка батареи
    // Добавьте другие поля по мере необходимости
};

void initEeprom();
void saveSettingsEeprom();
void eepromLoadSettings();
int getBacklightPercentEeprom();
void setBacklightPercentEeprom(int percent);
bool getBatteryEnabledEeprom();
void setBatteryEnabledEeprom(bool enabled);
