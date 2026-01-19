#include "hold-navigate.h"

#include <Arduino.h>

static bool s_isUpHeld = false;
static bool s_isDownHeld = false;
static unsigned long holdStartTime = 0;
static unsigned long lastStepTime = 0;

void handleHoldNavigation(bool isUpHeld, bool isDownHeld, void (*onStep)(bool isUp), uint16_t stepIntervalMs) {
    static bool wasUp = false;
    static bool wasDown = false;

    unsigned long now = millis();

    // Обнаружение начала удержания
    if (isUpHeld && !wasUp) {
        holdStartTime = now;
        s_isUpHeld = true;
        s_isDownHeld = false;
    } else if (isDownHeld && !wasDown) {
        holdStartTime = now;
        s_isDownHeld = true;
        s_isUpHeld = false;
    }

    // Сброс при отпускании
    if (!isUpHeld && !isDownHeld) {
        s_isUpHeld = false;
        s_isDownHeld = false;
        lastStepTime = 0;
    }

    // Автопрокрутка
    if ((s_isUpHeld || s_isDownHeld) && (now - holdStartTime) >= HOLD_THRESHOLD_MS) {
        if (now - lastStepTime >= stepIntervalMs) {
            lastStepTime = now;
            if (s_isUpHeld) onStep(true);
            if (s_isDownHeld) onStep(false);
        }
    }

    wasUp = isUpHeld;
    wasDown = isDownHeld;
}
