#pragma once

#include <Arduino.h>

/// Инициализация батареи
void initBattery();

/// Получить напряжение батареи (В)
float getBatteryVoltage();

/// Получить процент заряда батареи (0-100)
int getBatteryPercent();

/// Получить сырое значение ADC
int getBatteryAdcValue();

/// Проверить, низкий ли заряд (<=3.0V)
bool isBatteryLow();

/// Проверить, нужно ли затемнять подсветку (<20%)
bool shouldDimBacklight();

/// Обновление батареи (для периодических задач)
void updateBattery();