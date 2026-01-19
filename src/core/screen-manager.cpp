#include "screen-manager.h"

#include "drivers/lcd-driver.h"

// Экраны
#include "screens/screen-about.h"
#include "screens/screen-backlight-edit.h"
#include "screens/screen-diagnostics.h"
#include "screens/screen-list.h"
#include "screens/screen-settings.h"

static bool isInitScreen[SCREEN_COUNT] = {false};

static void initScreen(ScreenId id) {
    if (isInitScreen[id]) return;

    switch (id) {
        case SCREEN_LIST:
            initArrowsLCD();
            break;
        case SCREEN_SETTINGS:
            initScreenSettings();
            initArrowsLCD();
            break;
        case SCREEN_BACKLIGHT_EDIT:
            initScreenBacklightEdit();
            initArrowsLCD();
            break;
        case SCREEN_DIAGNOSTICS:
            initScreenDiagnostics();
            break;
        case SCREEN_ABOUT:
            initAboutScreen();
            initArrowsLCD();
            break;
        default:
            break;
    }
    isInitScreen[id] = true;
}

static ScreenId updateScreen(ScreenId id) {
    switch (id) {
        case SCREEN_LIST:
            return screenListUpdate();
        case SCREEN_SETTINGS:
            return updateScreenSettings();
        case SCREEN_BACKLIGHT_EDIT:
            if (updateScreenBacklightEdit()) return SCREEN_SETTINGS;
            return SCREEN_BACKLIGHT_EDIT;
        case SCREEN_DIAGNOSTICS:
            if (updateScreenDiagnostics()) return SCREEN_LIST;
            return SCREEN_DIAGNOSTICS;
        case SCREEN_ABOUT:
            if (updateAboutScreen()) return SCREEN_LIST;
            return SCREEN_ABOUT;
        default:
            return SCREEN_LIST;
    }
}

void updateScreenManager() {
    static ScreenId currentScreen = SCREEN_LIST;

    initScreen(currentScreen);
    ScreenId nextScreen = updateScreen(currentScreen);

    if (nextScreen != currentScreen) {
        isInitScreen[currentScreen] = false;
        if (nextScreen == SCREEN_LIST) {
            screenListRedraw();
        }
        currentScreen = nextScreen;
    }
}
