#pragma once

#include <Arduino.h>

// Инициализация EEPROM (вызывается один раз в setup)
void eepromInit();

// Сохранение текущих настроек
void eepromSaveSettings();

// Загрузка настроек
void eepromLoadSettings();

// Получить/установить яркость подсветки (0–100%)
uint8_t eepromGetBacklightPercent();
void eepromSetBacklightPercent(uint8_t percent);
