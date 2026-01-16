#include <Arduino.h>

#include "buttons.h"
#include "config.h"
#include "lcd-driver.h"
#include "screen-list.h"

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
        case SCREEN_SETTINGS:
            break;

        default:
            currentScreen = SCREEN_LIST;
            break;
    }
}
