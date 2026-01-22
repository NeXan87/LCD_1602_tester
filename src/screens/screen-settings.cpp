#include "screen-settings.h"

#include "core/screen-submenu.h"
#include "drivers/lcd-driver.h"
#include "screens/settings/screen-backlight-edit.h"
#include "screens/settings/screen-battery-edit.h"
#include "utils/helpers.h"

static const char* const SETTINGS_ITEMS[] = {
    "Display bridge",
    "Battery support",
};

static ScreenSubMenu g_settingsMenu = {
    .title = "Settings",
    .items = SETTINGS_ITEMS,
    .count = COUNT_OF(SETTINGS_ITEMS),
    .selectedIndex = 0,
    .screenId = SCREEN_SETTINGS,
    .exitScreen = SCREEN_LIST,
    .isInitialized = false,
    .initFunc = initArrowsLCD,
};

static ScreenId getSettingsItemScreen(int index) {
    switch (index) {
        case 0: return SCREEN_BACKLIGHT_EDIT;
        case 1: return SCREEN_BATTERY_EDIT;
        default: return SCREEN_SETTINGS;
    }
}

ScreenId updateScreenSettings() {
    return updateSubMenu(&g_settingsMenu, getSettingsItemScreen);
}
