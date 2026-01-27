#pragma once

#include "core/screen-id.h"
#include "drivers/encoder-driver.h"
#include "screens/encoder-tester/enum.h"

typedef void (*SubMenuInitFunc)(void);

typedef struct {
    uint8_t pin_plus;
    uint8_t pin_minus;
    int16_t last_diff;
    bool ever_high;  // был ли diff > порога
    bool ever_low;   // был ли diff < -порога
} DiffChannel;

typedef struct {
    DiffChannel a;
    DiffChannel b;
    DiffChannel r;
    uint32_t errors;
} DiffEncoderState;

typedef struct {
    EncoderInterfaces interface;
    EncoderType type;
    ScreenId screenId;
    ScreenId exitScreen;
    EncoderDisplayMode displayMode;
    EncoderDisplayMode displayModeCount;
    bool isInitialized;
    SubMenuInitFunc initFunc;
    unsigned long lastDrawTime;
    EncoderState* state;
} EncoderConfig;

ScreenId updateEncoderTestScreen(EncoderConfig* encoder);
