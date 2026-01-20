#include "screens/screen-encoder-ttl-test.h"

#include "drivers/buttons.h"
#include "drivers/encoder-driver.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static EncoderState encoderState;
static uint8_t displayMode = 0;  // 0 - диагностика, 1 - позиция/скорость
static unsigned long lastDrawTime = 0;

static void drawEncoderTestScreen() {
    clearLCD();
    setCursorLCD(0, 0);

    char buf[16];

    if (displayMode == 0) {
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
    } else {
        // Pos:12345
        // Dir:▶ S:100 i/s
        printLCD("Pos:");
        sprintf(buf, "%ld", encoderState.position);
        printLCD(buf);

        setCursorLCD(0, 1);
        printLCD("Dir:");
        if (encoderState.direction == 1) {
            printLCD("\x03");  // right arrow
        } else if (encoderState.direction == -1) {
            printLCD("\x04");  // left arrow
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
    initArrowsLCD();

    displayMode = 0;
    drawEncoderTestScreen();
    lastDrawTime = millis();
    // Пины: A D2, B D3, R D4
    initEncoder(2, 3, 11, &encoderState);
}

ScreenId updateScreenEncoderTTLTest() {
    updateEncoder(&encoderState);

    // Перерисовка экрана каждые 500 мс для отображения актуальных данных
    unsigned long now = millis();
    if (now - lastDrawTime > 500) {
        drawEncoderTestScreen();
        lastDrawTime = now;
    }

    // Переключение режима Up/Down
    if (clickUpButton()) {
        displayMode = (displayMode == 0) ? 1 : 0;
        Serial.println(displayMode);
        drawEncoderTestScreen();
        lastDrawTime = millis();
    }
    if (clickDownButton()) {
        displayMode = (displayMode == 0) ? 1 : 0;
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
