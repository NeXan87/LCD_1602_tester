#pragma once

#include "core/screen-id.h"
#include "drivers/encoder-driver.h"

typedef enum {
    ENCODER_DISPLAY_DIAGNOSTICS,
    ENCODER_DISPLAY_POSITION_SPEED,
    ENCODER_DISPLAY_WAVEFORM,
    ENCODER_DISPLAY_MODE_COUNT
} EncoderDisplayMode;

typedef enum {
    ENCODER_TYPE_ROTARY,  // Угловой (поворотный)
    ENCODER_TYPE_LINEAR   // Линейный
} EncoderType;

typedef enum {
    TTL,
} EncoderInterfaces;

typedef void (*SubMenuInitFunc)(void);

typedef struct {
    EncoderInterfaces interface;
    EncoderType type;
    ScreenId screenId;
    ScreenId exitScreen;
    EncoderDisplayMode displayMode;
    EncoderDisplayMode displayModeCount;
    bool isInitialized;
    SubMenuInitFunc initFunc;
    EncoderState* state;
    unsigned long lastDrawTime;
} EncoderConfig;

ScreenId updateEncoderTestScreen(EncoderConfig* encoder);
