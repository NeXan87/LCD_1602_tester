#include "screen-manager.h"

#include "drivers/lcd-driver.h"

// Экраны
#include "screens/screen-about.h"
#include "screens/screen-diagnostics.h"
#include "screens/screen-list.h"
#include "screens/screen-settings.h"

void screenManagerUpdate() {
    static ScreenId currentScreen = SCREEN_LIST;

    switch (currentScreen) {
        case SCREEN_LIST: {
            static bool listInitialized = false;
            if (!listInitialized) {
                listInitialized = true;
                initArrowsLCD();
            }
            ScreenId next = screenListUpdate();
            if (next != SCREEN_LIST) {
                currentScreen = next;
                listInitialized = false;
            }
            break;
        }
        case SCREEN_IR_TEST:
            break;
        case SCREEN_ENCODER_TEST:
            break;
        case SCREEN_SETTINGS: {
            static bool settingsInitialized = false;
            if (!settingsInitialized) {
                settingsInitialized = true;
                initScreenSettings();
                initArrowsLCD();
            }
            if (updateScreenSettings()) {
                currentScreen = SCREEN_LIST;
                settingsInitialized = false;
                screenListRedraw();
            }
            break;
        }
        case SCREEN_DIAGNOSTICS: {
            static bool diagInitialized = false;

            if (!diagInitialized) {
                diagInitialized = true;
                initScreenDiagnostics();
            }
            if (updateScreenDiagnostics()) {
                currentScreen = SCREEN_LIST;
                diagInitialized = false;
                screenListRedraw();
            }
            break;
        }
        case SCREEN_ABOUT: {
            static bool aboutInitialized = false;

            if (!aboutInitialized) {
                aboutInitialized = true;
                initArrowsLCD();
                initAboutScreen();
            }
            if (updateAboutScreen()) {
                currentScreen = SCREEN_LIST;
                aboutInitialized = false;
                screenListRedraw();
            }
            break;
        }
        default:
            currentScreen = SCREEN_LIST;
    }
}
