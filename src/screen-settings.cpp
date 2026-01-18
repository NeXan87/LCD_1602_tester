#include "screen-settings.h"

#include "buttons.h"
#include "config.h"
#include "eeprom2.h"
#include "lcd-custom-chars.h"
#include "lcd-driver.h"
#include "lcd-helpers.h"

static const char* const SETTINGS_ITEMS[] = {
    "Display bridge"};
static const uint8_t SETTINGS_COUNT = sizeof(SETTINGS_ITEMS) / sizeof(SETTINGS_ITEMS[0]);

typedef enum {
    SETTINGS_STATE_MENU,
    SETTINGS_STATE_EDIT_BACKLIGHT
} SettingsState;

static SettingsState state = SETTINGS_STATE_MENU;
static int menuSelectedIndex = 0;
static int backlightPercent;          // текущее значение в редакторе
static int originalBacklightPercent;  // исходное значение (для отмены)

static int percentToPwm(int percent) {
    return (percent * 255) / 100;
}

static void applyBacklight() {
    analogWrite(BACKLIGHT_PIN, percentToPwm(backlightPercent));
}

void setBacklightPercent() {
    backlightPercent = eepromGetBacklightPercent();
    applyBacklight();
}

static void drawSettingsMenu() {
    drawSubMenu("Settings", SETTINGS_ITEMS[menuSelectedIndex], menuSelectedIndex == 0);
}

static void drawBacklightEditor() {
    char buffer[17];
    snprintf(buffer, sizeof(buffer), "Bridge:%3d%%", backlightPercent);
    setCursorLCD(0, 0);
    printLCD(buffer);
    setCursorLCD(0, 1);
    writeCharLCD(LCD_CHAR_ARROW_UP);
    writeCharLCD(' ');
    writeCharLCD(LCD_CHAR_ARROW_DOWN);
    printLCD(" to adjust");
}

void initScreenSettings() {
    state = SETTINGS_STATE_MENU;
    menuSelectedIndex = 0;
    drawSettingsMenu();
}

bool updateScreenSettings() {
    switch (state) {
        case SETTINGS_STATE_MENU:
            if (updateMenuIndex(&menuSelectedIndex, SETTINGS_COUNT)) {
                drawSettingsMenu();
            }
            if (clickSelectButton()) {
                clearLCD();

                if (menuSelectedIndex == 0) {
                    originalBacklightPercent = eepromGetBacklightPercent();
                    backlightPercent = originalBacklightPercent;
                    applyBacklight();
                    state = SETTINGS_STATE_EDIT_BACKLIGHT;
                    drawBacklightEditor();
                }
            }
            break;

        case SETTINGS_STATE_EDIT_BACKLIGHT:
            if (clickDownButton()) {
                if (backlightPercent >= 10) {
                    backlightPercent -= 10;
                    applyBacklight();
                    drawBacklightEditor();
                }
            }
            if (clickUpButton()) {
                if (backlightPercent <= 90) {
                    backlightPercent += 10;
                    applyBacklight();
                    drawBacklightEditor();
                }
            }
            if (clickLeftButton()) {
                backlightPercent = originalBacklightPercent;
                applyBacklight();
                drawSettingsMenu();
                state = SETTINGS_STATE_MENU;
                return false;
            }
            if (clickSelectButton()) {
                eepromSetBacklightPercent(backlightPercent);
                eepromSaveSettings();
                drawSettingsMenu();
                state = SETTINGS_STATE_MENU;
                return false;
            }
            break;
    }

    if (state == SETTINGS_STATE_MENU && clickLeftButton()) {
        clearLCD();
        return true;
    }

    return false;
}
