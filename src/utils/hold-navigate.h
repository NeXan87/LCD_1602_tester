#pragma once

#include <stdint.h>

#include "config.h"
#include "drivers/buttons.h"

/// Управление авто прокруткой при удержании кнопки
/// @param upHeld — результат clickUpButton() ИЛИ isUpHeld()
/// @param downHeld — результат clickDownButton() ИЛИ isDownHeld()
/// @param onStep — callback, вызываемый при каждом шаге прокрутки
/// @param stepIntervalMs — интервал между шагами после удержания (например, 200 мс)
void handleHoldNavigation(bool upHeld, bool downHeld, void (*onStep)(bool isUp), uint16_t stepIntervalMs = STEP_INTERVAL_DEFAULT_MS);
