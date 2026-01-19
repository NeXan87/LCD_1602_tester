#pragma once
#include <Arduino.h>

#include "config.h"

/// Инициализация подсветки (вызывается один раз в setup)
void initBacklight();

/// Установка яркости (0–100%)
void setBacklightPercent(int percent);

/// Плавная установка яркости (0–100%, durationMs - время в мс)
void setBacklightPercentSmooth(int targetPercent, unsigned long durationMs = BACKLIGHT_FADE_MS);

/// Обновление плавного изменения яркости (вызывать в loop)
void updateBacklight();

/// Применение сохранённой яркости из EEPROM
void saveApplyBacklight();
