#pragma once

#include <stdint.h>

#include "config.h"
#include "drivers/buttons.h"

/// Управление авто прокруткой при удержании кнопки
/// @param isUpHeld — результат clickUpButton() ИЛИ isUpHeld()
/// @param isDownHeld — результат clickDownButton() ИЛИ isDownHeld()
/// @param onStep — callback, вызываемый при каждом шаге прокрутки, возвращает true если произошло изменение
/// @param stepIntervalMs — интервал между шагами после удержания (например, 200 мс)
/// @return true если произошло изменение
bool handleHoldNavigation(bool isUpHeld, bool isDownHeld, bool (*onStep)(bool isUp), uint16_t stepIntervalMs = STEP_INTERVAL_DEFAULT_MS);
