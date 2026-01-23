#include "hold-navigate.h"

#include <Arduino.h>

static bool s_isUpHeld = false;
static bool s_isDownHeld = false;
static unsigned long holdStartTime = 0;
static unsigned long lastStepTime = 0;

bool handleHoldNavigation(HoldNavigateCallback onStep, void* userData, uint16_t stepIntervalMs) {
    static bool wasUp = false;
    static bool wasDown = false;

    unsigned long now = millis();
    bool isChanged = false;

    if (isUpButtonPressed() && !wasUp) {
        holdStartTime = now;
        s_isUpHeld = true;
        s_isDownHeld = false;
    } else if (isDownButtonPressed() && !wasDown) {
        holdStartTime = now;
        s_isDownHeld = true;
        s_isUpHeld = false;
    }

    if (!isUpButtonPressed() && !isDownButtonPressed()) {
        s_isUpHeld = false;
        s_isDownHeld = false;
        lastStepTime = 0;
    }

    if ((s_isUpHeld || s_isDownHeld) && (now - holdStartTime) >= HOLD_THRESHOLD_MS) {
        if (now - lastStepTime >= stepIntervalMs) {
            lastStepTime = now;
            if (s_isUpHeld) {
                if (onStep(s_isUpHeld, userData)) isChanged = true;
            }
            if (s_isDownHeld) {
                if (onStep(!s_isDownHeld, userData)) isChanged = true;
            }
        }
    }

    wasUp = isUpButtonPressed();
    wasDown = isDownButtonPressed();

    return isChanged;
}
