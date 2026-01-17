#include "screen-settings.h"

#include "buttons.h"
#include "config.h"
#include "lcd-driver.h"

static const char* const SETTINGS_ITEMS[] = {
    "Display bridge"};
static const uint8_t SETTINGS_COUNT = sizeof(SETTINGS_ITEMS) / sizeof(SETTINGS_ITEMS[0]);

typedef enum {
    SETTINGS_STATE_MENU,
    SETTINGS_STATE_EDIT_BACKLIGHT
} SettingsState;

static SettingsState state = SETTINGS_STATE_MENU;
static int menuSelectedIndex = 0;
static int backlightPercent = 100;  // по умолчанию 100%

static int percentToPwm(int percent) {
    return (percent * 255) / 100;
}

static void applyBacklight() {
    analogWrite(BACKLIGHT_PIN, percentToPwm(backlightPercent));
}

static void drawSettingsMenu() {
    clearLCD();
    setCursorLCD(0, 0);
    printLCD("Settings");
    if (SETTINGS_COUNT > 0) {
        setCursorLCD(0, 1);
        if (menuSelectedIndex == 0) {
            writeCharLCD(126);
        } else {
            printLCD(" ");
        }
        printLCD(SETTINGS_ITEMS[menuSelectedIndex]);
    }
}

static void drawBacklightEditor() {
    char buffer[17];
    snprintf(buffer, sizeof(buffer), "Bridge:%3d%%", backlightPercent);
    clearLCD();
    setCursorLCD(0, 0);
    printLCD(buffer);
    setCursorLCD(0, 1);
    writeCharLCD(127);
    writeCharLCD(' ');
    writeCharLCD(126);
    printLCD(" to adjust");
}

void initScreenSettings() {
    state = SETTINGS_STATE_MENU;
    menuSelectedIndex = 0;
    // Загрузить текущее значение из EEPROM (опционально)
    applyBacklight();
    drawSettingsMenu();
}

bool updateScreenSettings() {
    switch (state) {
        case SETTINGS_STATE_MENU:
            if (clickUpButton()) {
                if (menuSelectedIndex > 0) {
                    menuSelectedIndex--;
                    drawSettingsMenu();
                }
            }
            if (clickDownButton()) {
                if (menuSelectedIndex < SETTINGS_COUNT - 1) {
                    menuSelectedIndex++;
                    drawSettingsMenu();
                }
            }
            if (clickSelectButton()) {
                if (menuSelectedIndex == 0) {
                    state = SETTINGS_STATE_EDIT_BACKLIGHT;
                    drawBacklightEditor();
                }
            }
            break;

        case SETTINGS_STATE_EDIT_BACKLIGHT:
            if (clickLeftButton()) {
                if (backlightPercent >= 10) {
                    backlightPercent -= 10;
                    applyBacklight();
                    drawBacklightEditor();
                }
            }
            if (clickRightButton()) {
                if (backlightPercent <= 90) {
                    backlightPercent += 10;
                    applyBacklight();
                    drawBacklightEditor();
                }
            }
            if (clickSelectButton()) {
                // Сохранить в EEPROM (опционально)
                state = SETTINGS_STATE_MENU;
                drawSettingsMenu();
                return false;
            }
            break;
    }

    if (state == SETTINGS_STATE_MENU && clickLeftButton()) {
        return true;
    }

    return false;
}
