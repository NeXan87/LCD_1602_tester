#include "screen-diagnostics.h"

#include "buttons.h"
#include "lcd-driver.h"
#include "lcd-helpers.h"

static const char* const DIAG_MENU_ITEMS[] = {
    "Buttons",
};
static const uint8_t DIAG_MENU_COUNT = sizeof(DIAG_MENU_ITEMS) / sizeof(DIAG_MENU_ITEMS[0]);
static bool isInitButtonsTest = false;

typedef enum {
    DIAG_STATE_MENU,
    DIAG_STATE_BUTTONS_TEST
} DiagState;

static DiagState state = DIAG_STATE_MENU;
static uint8_t menuSelectedIndex = 0;

const char* getButtonName(int adcValue) {
    if (adcValue < BUTTON_RIGHT_ADC + 50) return "Right";
    if (adcValue < BUTTON_UP_ADC + 50) return "Up";
    if (adcValue < BUTTON_DOWN_ADC + 100) return "Down";
    if (adcValue < BUTTON_LEFT_ADC + 100) return "Left";
    if (adcValue < BUTTON_SELECT_ADC + 100) return "Select";
    return "None";
}

static void drawDiagMenu() {
    drawSubMenu("Diagnostics", DIAG_MENU_ITEMS[menuSelectedIndex], menuSelectedIndex == 0);
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

void initScreenDiagnostics() {
    state = DIAG_STATE_MENU;
    menuSelectedIndex = 0;
    drawDiagMenu();
}

bool updateScreenDiagnostics() {
    switch (state) {
        case DIAG_STATE_MENU: {
            if (clickUpButton()) {
                if (menuSelectedIndex > 0) {
                    menuSelectedIndex--;
                    drawDiagMenu();
                }
            }
            if (clickDownButton()) {
                if (menuSelectedIndex < DIAG_MENU_COUNT - 1) {
                    menuSelectedIndex++;
                    drawDiagMenu();
                }
            }
            if (clickSelectButton()) {
                if (menuSelectedIndex == 0) {
                    state = DIAG_STATE_BUTTONS_TEST;
                    drawButtonsTest();
                }
            }
            break;
        }

        case DIAG_STATE_BUTTONS_TEST: {
            static uint32_t lastUpdate = 0;
            if (millis() - lastUpdate > DIAG_BUTTONS_UPDATE_MS) {
                lastUpdate = millis();
                drawButtonsTest();
            }

            if (isLeftButtonHeld()) {
                state = DIAG_STATE_MENU;
                isInitButtonsTest = false;
                lastUpdate = 0;
                drawDiagMenu();
                return false;
            }
            break;
        }
    }

    if (state == DIAG_STATE_MENU && clickLeftButton()) {
        clearLCD();
        return true;
    }

    return false;
}
