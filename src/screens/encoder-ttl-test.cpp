#include "screens/encoder-ttl-test.h"

#include "drivers/buttons.h"
#include "drivers/encoder-driver.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static EncoderState encoderState;
static EncoderDisplayMode displayMode = ENCODER_DISPLAY_DIAGNOSTICS;
static unsigned long lastDrawTime = 0;

static void drawEncoderTestScreen() {
    clearLCD();
    setCursorLCD(0, 0);

    char buf[16];

    if (displayMode == ENCODER_DISPLAY_DIAGNOSTICS) {
        // A:OK B:OK R:✓
        // ERR:0
        printLCD("A:");
        printLCD(encoderState.aEverHigh ? "OK" : "NO");
        printLCD(" B:");
        printLCD(encoderState.bEverHigh ? "OK" : "NO");
        printLCD(" R:");
        printLCD(encoderState.rPresent ? "\x02" : " ");

        setCursorLCD(0, 1);
        printLCD("ERR:");
        sprintf(buf, "%lu", encoderState.errors);
        printLCD(buf);
    } else if (displayMode == ENCODER_DISPLAY_POSITION_SPEED) {
        // Pos:12345
        // Dir:▶ S:100 i/s
        printLCD("Pos:");
        sprintf(buf, "%ld", encoderState.position);
        printLCD(buf);

        setCursorLCD(0, 1);
        printLCD("Dir:");
        if (encoderState.direction == 1) {
            writeCharLCD(126);  // right arrow
        } else if (encoderState.direction == -1) {
            writeCharLCD(127);  // left arrow
        } else {
            printLCD("-");
        }
        printLCD(" S:");
        sprintf(buf, "%d", encoderState.speed);
        printLCD(buf);
        printLCD(" i/s");
    }
}

void initScreenEncoderTTLTest() {
    displayMode = ENCODER_DISPLAY_DIAGNOSTICS;
    lastDrawTime = millis();
    initArrowsLCD();
    drawEncoderTestScreen();
    initEncoder(&encoderState);
}

ScreenId updateScreenEncoderTTLTest() {
    updateEncoder(&encoderState);

    // Перерисовка экрана каждые 500 мс для отображения актуальных данных
    unsigned long now = millis();
    if (now - lastDrawTime > ENCODER_DRAW_INTERVAL_MS) {
        drawEncoderTestScreen();
        lastDrawTime = now;
    }

    // Переключение режима Up/Down
    if (clickUpButton()) {
        displayMode = (displayMode == ENCODER_DISPLAY_DIAGNOSTICS ? ENCODER_DISPLAY_POSITION_SPEED : ENCODER_DISPLAY_DIAGNOSTICS);
        Serial.println(displayMode);
        drawEncoderTestScreen();
        lastDrawTime = millis();
    }
    if (clickDownButton()) {
        displayMode = (displayMode == ENCODER_DISPLAY_DIAGNOSTICS ? ENCODER_DISPLAY_POSITION_SPEED : ENCODER_DISPLAY_DIAGNOSTICS);
        Serial.println(displayMode);
        drawEncoderTestScreen();
        lastDrawTime = millis();
    }

    // Select для сброса
    if (clickSelectButton()) {
        resetEncoder(&encoderState);
        drawEncoderTestScreen();
        lastDrawTime = millis();
    }

    // Left для выхода
    if (clickLeftButton()) {
        clearLCD();
        return SCREEN_ENCODER_TEST;
    }

    return SCREEN_ENCODER_TEST_TTL;
}
