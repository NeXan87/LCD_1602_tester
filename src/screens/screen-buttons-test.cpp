#include "screen-buttons-test.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static bool isInitButtonsTest = false;

const char* getButtonName(int adcValue) {
    if (adcValue < BUTTON_RIGHT_ADC + 50) return "Right";
    if (adcValue < BUTTON_UP_ADC + 50) return "Up";
    if (adcValue < BUTTON_DOWN_ADC + 100) return "Down";
    if (adcValue < BUTTON_LEFT_ADC + 100) return "Left";
    if (adcValue < BUTTON_SELECT_ADC + 100) return "Select";
    return "None";
}

static void drawButtonsTest() {
    if (!isInitButtonsTest) {
        clearLCD();
        setCursorLCD(0, 0);
        printLCD("Press buttons:");
        isInitButtonsTest = true;
    }

    int adcVal = analogRead(BUTTONS_PIN);
    const char* btnName = getButtonName(adcVal);

    char line[17];

    snprintf(line, sizeof(line), "%-7s ADC:%-4d", btnName, adcVal);
    setCursorLCD(0, 1);
    printLCD(line);
}

void initScreenButtonsTest() {
    drawButtonsTest();
}

ScreenId updateScreenButtonsTest() {
    static uint32_t lastUpdate = 0;
    if (millis() - lastUpdate > DIAG_BUTTONS_UPDATE_MS) {
        lastUpdate = millis();
        drawButtonsTest();
    }

    if (isLeftButtonHeld()) {
        isInitButtonsTest = false;
        lastUpdate = 0;
        return SCREEN_DIAGNOSTICS;
    }

    return SCREEN_BUTTONS_TEST;
}
