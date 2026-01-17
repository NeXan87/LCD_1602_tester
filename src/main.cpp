#include <Arduino.h>

#include "buttons.h"
#include "config.h"
#include "lcd-driver.h"
#include "screen-about.h"
#include "screen-diagnostics.h"
#include "screen-list.h"
#include "screen-settings.h"

void setup() {
    initLCD();
    screenListRedraw();
    Serial.begin(115200);
}

void loop() {
    static ScreenId currentScreen = SCREEN_LIST;
    updateButtons();

    switch (currentScreen) {
        case SCREEN_LIST: {
            ScreenId next = screenListUpdate();
            if (next != SCREEN_LIST) {
                currentScreen = next;
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
                initCharsAboutScreen();
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
