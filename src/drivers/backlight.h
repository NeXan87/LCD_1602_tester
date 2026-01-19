#pragma once

#include <Arduino.h>

/// Инициализация подсветки (вызывается один раз в setup)
void backlightInit();

/// Установка яркости (0–100%)
void backlightSetPercent(int percent);

/// Применение сохранённой яркости из EEPROM
void backlightApplySaved();
