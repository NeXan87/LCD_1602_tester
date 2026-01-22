#include "screen-manager.h"

#include "drivers/lcd-driver.h"
#include "utils/helpers.h"

// Экраны
#include "screens/about.h"
#include "screens/diagnostics.h"
#include "screens/diagnostics/battery-test.h"
#include "screens/diagnostics/buttons-test.h"
#include "screens/encoder-test.h"
#include "screens/encoder-ttl-test.h"
#include "screens/main-menu.h"
#include "screens/settings.h"
#include "screens/settings/backlight-edit.h"
#include "screens/settings/battery-edit.h"

static bool isInitScreen[SCREEN_COUNT] = {false};

static void initScreen(ScreenId id) {
    if (isInitScreen[id]) return;

    switch (id) {
        case SCREEN_ENCODER_TEST_TTL:
            initScreenEncoderTTLTest();
            break;
        case SCREEN_BACKLIGHT_EDIT:
            initScreenBacklightEdit();
            initArrowsLCD();
            break;
        case SCREEN_BATTERY_EDIT:
            initScreenBatteryEdit();
            break;
        case SCREEN_BUTTONS_TEST:
            initScreenButtonsTest();
            break;
        case SCREEN_BATTERY_TEST:
            initScreenBatteryTest();
            break;
        default:
            break;
    }
    isInitScreen[id] = true;
}

static ScreenId updateScreen(ScreenId id) {
    switch (id) {
        case SCREEN_MAIN_MENU:
            return updateMainMenu();
        case SCREEN_ENCODER_TEST:
            return updateScreenEncoderTest();
        case SCREEN_ENCODER_TEST_TTL:
            return updateScreenEncoderTTLTest();
        case SCREEN_SETTINGS:
            return updateScreenSettings();
        case SCREEN_BACKLIGHT_EDIT:
            return updateScreenBacklightEdit();
        case SCREEN_BATTERY_EDIT:
            return updateScreenBatteryEdit();
        case SCREEN_DIAGNOSTICS:
            return updateScreenDiagnostics();
        case SCREEN_BUTTONS_TEST:
            return updateScreenButtonsTest();
        case SCREEN_BATTERY_TEST:
            return updateScreenBatteryTest();
        case SCREEN_ABOUT:
            return updateAboutScreen();
        default:
            return SCREEN_MAIN_MENU;
    }
}

void updateScreenManager() {
    static ScreenId currentScreen = SCREEN_MAIN_MENU;

    initScreen(currentScreen);
    ScreenId nextScreen = updateScreen(currentScreen);

    if (nextScreen != currentScreen) {
        isInitScreen[currentScreen] = false;
        if (nextScreen == SCREEN_MAIN_MENU) {
            redrawMainMenu();
        }
        currentScreen = nextScreen;
    }
}
