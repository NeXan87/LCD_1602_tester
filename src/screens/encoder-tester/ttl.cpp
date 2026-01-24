#include "screens/encoder-tester/ttl.h"

#include <string.h>

#include "drivers/buttons.h"
#include "drivers/encoder-driver.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static EncoderState encoderState;
static EncoderDisplayMode displayMode = ENCODER_DISPLAY_DIAGNOSTICS;
static unsigned long lastDrawTime = 0;
static bool isInitialized = false;

static void drawEncoderTestScreen() {
    setCursorLCD(0, 0);

    if (displayMode == ENCODER_DISPLAY_DIAGNOSTICS) {
        // Строка 0: A:OK B:OK R:✓
        const char* aStatus = encoderState.aEverHigh ? "OK" : "NO";
        const char* bStatus = encoderState.bEverHigh ? "OK" : "NO";
        const char* rStatus = encoderState.rPresent ? "\x04" : " ";
        printfLCD("A:%s B:%s R:%s", aStatus, bStatus, rStatus);

        setCursorLCD(0, 1);
        // Строка 1: ERR:12345
        printfLCD("ERR:%lu", encoderState.errors);
    } else if (displayMode == ENCODER_DISPLAY_POSITION_SPEED) {
        // Строка 0: Pos:12345
        printfLCD("Pos:%-12ld", encoderState.position);

        setCursorLCD(0, 1);
        // Строка 1: Dir:▶ S:100 i/s
        char dirChar = '-';
        if (encoderState.direction == 1) {
            dirChar = 126;  // right arrow
        } else if (encoderState.direction == -1) {
            dirChar = 127;  // left arrow
        }
        printfLCD("Dir:%c S:%di/s     ", dirChar, encoderState.speed);
    } else if (displayMode == ENCODER_DISPLAY_WAVEFORM) {
        printLCD("A:");
        for (uint8_t i = 0; i < WAVEFORM_HISTORY_SIZE; i++) {
            int idx = (encoderState.historyIndex + i) % WAVEFORM_HISTORY_SIZE;
            writeCharLCD((uint8_t)encoderState.historyA[idx]);
        }

        setCursorLCD(0, 1);
        printLCD("B:");
        for (uint8_t i = 0; i < WAVEFORM_HISTORY_SIZE; i++) {
            int idx = (encoderState.historyIndex + i) % WAVEFORM_HISTORY_SIZE;
            writeCharLCD((uint8_t)encoderState.historyB[idx]);
        }
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
