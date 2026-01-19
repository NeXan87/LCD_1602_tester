#pragma once

#include <Arduino.h>

/// Инициализация подсветки (вызывается один раз в setup)
void initBacklight();

/// Установка яркости (0–100%)
void setBacklightPercent(int percent);

/// Применение сохранённой яркости из EEPROM
void saveApplyBacklight();
