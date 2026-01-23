#include "screen-manager.h"

#include "drivers/lcd-driver.h"
#include "utils/helpers.h"

// Экраны
#include "screens/about.h"
#include "screens/diagnostics.h"
#include "screens/diagnostics/battery.h"
#include "screens/diagnostics/buttons.h"
#include "screens/encoder-tester.h"
#include "screens/encoder-tester/ttl.h"
#include "screens/main-menu.h"
#include "screens/settings.h"
#include "screens/settings/backlight.h"
#include "screens/settings/battery.h"

static ScreenId updateScreen(ScreenId id) {
    switch (id) {
        case SCREEN_MAIN_MENU:
            return updateMainMenu();

        // ───────────── Уровень 1: Главное меню ─────────────
        case SCREEN_IR_TEST:
        case SCREEN_ENCODER_TEST:
            return updateScreenEncoderTest();
        case SCREEN_SETTINGS:
            return updateScreenSettings();
        case SCREEN_DIAGNOSTICS:
            return updateScreenDiagnostics();
        case SCREEN_ABOUT:
            return updateAboutScreen();

        // ───────────── Уровень 2: Settings ─────────────
        case SCREEN_BACKLIGHT_EDIT:
            return updateScreenBacklightEdit();
        case SCREEN_BATTERY_EDIT:
            return updateScreenBatteryEdit();

        // ───────────── Уровень 2: Diagnostic ─────────────
        case SCREEN_BUTTONS_DIAG:
            return updateScreenButtonsDiag();
        case SCREEN_BATTERY_DIAG:
            return updateScreenBatteryDiag();

        // ───────────── Уровень 2: Тесты энкодеров ─────────────
        case SCREEN_ENCODER_TEST_TTL:
            return updateScreenEncoderTTLTest();

        default:
            return SCREEN_MAIN_MENU;
    }
}

void updateScreenManager() {
    static ScreenId currentScreen = SCREEN_MAIN_MENU;
    ScreenId nextScreen = updateScreen(currentScreen);

    if (nextScreen != currentScreen) {
        if (nextScreen == SCREEN_MAIN_MENU) {
            redrawMainMenu();
        }
        currentScreen = nextScreen;
    }
}
