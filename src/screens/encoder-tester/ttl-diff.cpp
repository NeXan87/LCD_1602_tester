#include "core/encoder-test.h"
#include "drivers/lcd-driver.h"

static EncoderState g_encoderState;

// Конфигурация экрана
static EncoderConfig g_encoderConfig = {
    .interface = TTL_DIFF,
    .type = ENCODER_TYPE_ROTARY,
    .screenId = SCREEN_ENCODER_TEST_TTL_DIFF,
    .exitScreen = SCREEN_ENCODER_TEST,
    .displayMode = ENCODER_DISPLAY_DIAGNOSTICS,
    .displayModeCount = ENCODER_DISPLAY_MODE_COUNT,
    .isInitialized = false,
    .initFunc = initEncoderCharLCD,
    .lastDrawTime = 0,
    .state = &g_encoderState,
};

ScreenId updateScreenEncoderDiff() {
    return updateEncoderTestScreen(&g_encoderConfig);
}
