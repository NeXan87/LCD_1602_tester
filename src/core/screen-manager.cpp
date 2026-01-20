#include "screen-manager.h"

#include "drivers/lcd-driver.h"

// Экраны
#include "screens/diagnostics/screen-battery-test.h"
#include "screens/diagnostics/screen-buttons-test.h"
#include "screens/screen-about.h"
#include "screens/screen-diagnostics.h"
#include "screens/screen-encoder-test.h"
#include "screens/screen-encoder-ttl-test.h"
#include "screens/screen-list.h"
#include "screens/screen-settings.h"
#include "screens/settings/screen-backlight-edit.h"
#include "screens/settings/screen-battery-edit.h"

static bool isInitScreen[SCREEN_COUNT] = {false};

static void initScreen(ScreenId id) {
    if (isInitScreen[id]) return;

    switch (id) {
        case SCREEN_LIST:
            initArrowsLCD();
            break;
        case SCREEN_ENCODER_TEST:
            initScreenEncoderTest();
            initArrowsLCD();
            break;
        case SCREEN_ENCODER_TEST_TTL:
            initScreenEncoderTTLTest();
            break;
        case SCREEN_SETTINGS:
            initScreenSettings();
            initArrowsLCD();
            break;
        case SCREEN_BACKLIGHT_EDIT:
            initScreenBacklightEdit();
            initArrowsLCD();
            break;
        case SCREEN_BATTERY_EDIT:
            initScreenBatteryEdit();
            break;
        case SCREEN_DIAGNOSTICS:
            initScreenDiagnostics();
            break;
        case SCREEN_BUTTONS_TEST:
            initScreenButtonsTest();
            break;
        case SCREEN_BATTERY_TEST:
            initScreenBatteryTest();
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
            return updateScreenList();
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
            redrawScreenList();
        }
        currentScreen = nextScreen;
    }
}
