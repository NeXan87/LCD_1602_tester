#pragma once

#include <stdint.h>

#include "config.h"
#include "drivers/buttons.h"

typedef bool (*HoldNavigateCallback)(bool isUp, void* userData);

/// @brief Управление автопрокруткой при удержании кнопок навигации
/// @param isInc true, если удерживается кнопка увеличения(должна возвращать true всё время удержания)
/// @param isDec true, если удерживается кнопка уменьшения(должна возвращать true всё время удержания)
/// @param onStep callback-функция, вызываемая при каждом шаге автопрокрутки.
///        Принимает направление (true = вверх) и указатель на пользовательские данные.
///        Должна вернуть true, если состояние изменилось и требуется перерисовка.
/// @param userData указатель на контекст (например, структуру состояния экрана), передаётся в onStep
/// @param stepIntervalMs интервал между шагами после начала автопрокрутки (по умолчанию STEP_INTERVAL_DEFAULT_MS)
/// @return true, если в результате автопрокрутки произошло изменение (onStep вернул true)
bool handleHoldNavigation(
    bool isInc,
    bool isDec,
    HoldNavigateCallback onStep,
    void* userData,
    uint16_t stepIntervalMs = STEP_INTERVAL_DEFAULT_MS);
