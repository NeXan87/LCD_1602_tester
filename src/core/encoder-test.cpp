#include "core/encoder-test.h"

#include <string.h>

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "screens/encoder-tester/display-modes.h"
#include "utils/lcd-helpers.h"

static void drawEncoderTestScreen(EncoderConfig* encoder) {
    switch (encoder->displayMode) {
        case ENCODER_DISPLAY_DIAGNOSTICS:
            drawDiagnostics(encoder->state);
            break;
        case ENCODER_DISPLAY_POSITION_SPEED:
            drawPositionSpeed(encoder->state);
            break;
        case ENCODER_DISPLAY_WAVEFORM:
            drawWaveform(encoder->state);
            break;
        default:
            printLCD("Unsupported mode");
            break;
    }
}

ScreenId updateEncoderTestScreen(EncoderConfig* encoder) {
    if (!encoder->isInitialized) {
        initEncoder(encoder->state);
        encoder->isInitialized = true;
        encoder->lastDrawTime = millis();

        if (encoder->initFunc != NULL) {
            encoder->initFunc();
        }

        drawEncoderTestScreen(encoder);
    }

    updateEncoder(encoder->state);

    unsigned long now = millis();
    if (now - encoder->lastDrawTime > ENCODER_DRAW_INTERVAL_MS) {
        drawEncoderTestScreen(encoder);
        encoder->lastDrawTime = now;
    }

    if (clickUpButton() || clickDownButton()) {
        encoder->displayMode = (EncoderDisplayMode)((encoder->displayMode + 1) % encoder->displayModeCount);
        clearLCD();
        drawEncoderTestScreen(encoder);
        encoder->lastDrawTime = millis();
    }

    if (clickSelectButton()) {
        resetEncoder(encoder->state);
        encoder->lastDrawTime = millis();
    }

    if (clickLeftButton()) {
        clearLCD();
        detachEncoder();
        encoder->isInitialized = false;
        return encoder->exitScreen;
    }

    return encoder->screenId;
}
