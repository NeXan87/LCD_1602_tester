#include "screens/encoder-tester/ttl.h"

#include <string.h>

#include "drivers/buttons.h"
#include "drivers/encoder-driver.h"
#include "drivers/lcd-driver.h"
#include "screens/encoder-tester/display-modes.h"
#include "utils/lcd-helpers.h"

static EncoderState encoderState;
static EncoderDisplayMode displayMode = ENCODER_DISPLAY_DIAGNOSTICS;
static unsigned long lastDrawTime = 0;
static bool isInitialized = false;

static void drawEncoderTestScreen() {
    switch (displayMode) {
        case ENCODER_DISPLAY_DIAGNOSTICS:
            drawDiagnostics(&encoderState);
            break;
        case ENCODER_DISPLAY_POSITION_SPEED:
            drawPositionSpeed(&encoderState);
            break;
        case ENCODER_DISPLAY_WAVEFORM:
            drawWaveform(&encoderState);
            break;
    }
}

void initScreenEncoderTTLTest() {
    displayMode = ENCODER_DISPLAY_DIAGNOSTICS;
    lastDrawTime = millis();
    isInitialized = true;
    initEncoderCharLCD();
    drawEncoderTestScreen();
    initEncoder(&encoderState);
}

ScreenId updateScreenEncoderTTLTest() {
    if (!isInitialized) {
        initScreenEncoderTTLTest();
    }

    updateEncoder(&encoderState);

    unsigned long now = millis();
    if (now - lastDrawTime > ENCODER_DRAW_INTERVAL_MS) {
        drawEncoderTestScreen();
        lastDrawTime = now;
    }

    if (clickUpButton() || clickDownButton()) {
        displayMode = (EncoderDisplayMode)((displayMode + 1) % 3);
        clearLCD();
        drawEncoderTestScreen();
        lastDrawTime = millis();
    }

    if (clickSelectButton()) {
        resetEncoder(&encoderState);
        lastDrawTime = millis();
    }

    if (clickLeftButton()) {
        clearLCD();
        isInitialized = false;
        resetEncoder(&encoderState);
        return SCREEN_ENCODER_TEST;
    }

    return SCREEN_ENCODER_TEST_TTL;
}
