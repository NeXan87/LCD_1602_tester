#pragma once

#include "core/screen-id.h"

typedef enum {
    ENCODER_DISPLAY_DIAGNOSTICS,
    ENCODER_DISPLAY_POSITION_SPEED
} EncoderDisplayMode;

// Обновление экрана TTL тестера энкодера
// @return ScreenId следующего экрана
ScreenId updateScreenEncoderTTLTest();
