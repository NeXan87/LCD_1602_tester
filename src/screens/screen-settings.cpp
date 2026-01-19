#include "screens/screen-settings.h"

#include "config.h"
#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static const char* const SETTINGS_ITEMS[] = {
    "Display bridge"};
static const uint8_t SETTINGS_COUNT = sizeof(SETTINGS_ITEMS) / sizeof(SETTINGS_ITEMS[0]);
static int menuSelectedIndex = 0;

static void drawSettingsMenu() {
    drawSubMenu("Settings", SETTINGS_ITEMS[menuSelectedIndex], menuSelectedIndex == 0);
}

void initScreenSettings() {
    menuSelectedIndex = 0;
    drawSettingsMenu();
}

ScreenId updateScreenSettings() {
    if (updateMenuIndex(&menuSelectedIndex, SETTINGS_COUNT)) {
        drawSettingsMenu();
    }
    if (clickSelectButton()) {
        if (menuSelectedIndex == 0) {
            clearLCD();
            return SCREEN_BACKLIGHT_EDIT;
        }
    }
    if (clickLeftButton()) {
        clearLCD();
        return SCREEN_LIST;
    }
    return SCREEN_SETTINGS;
}
