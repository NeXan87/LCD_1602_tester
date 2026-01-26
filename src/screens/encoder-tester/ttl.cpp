#include "ttl.h"

#include "core/encoder-test.h"
#include "drivers/lcd-driver.h"

static EncoderState g_encoderState;
static EncoderConfig g_encoderConfig = {
    .interface = TTL,
    .type = ENCODER_TYPE_ROTARY,
    .screenId = SCREEN_ENCODER_TEST_TTL,
    .exitScreen = SCREEN_ENCODER_TEST,
    .displayMode = ENCODER_DISPLAY_DIAGNOSTICS,
    .displayModeCount = ENCODER_DISPLAY_MODE_COUNT,
    .isInitialized = false,
    .initFunc = initEncoderCharLCD,
    .state = &g_encoderState,
    .lastDrawTime = 0,
};

ScreenId updateScreenEncoderTTL() {
    return updateEncoderTestScreen(&g_encoderConfig);
}
