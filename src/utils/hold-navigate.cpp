#include "hold-navigate.h"

#include <Arduino.h>

static bool s_upHeld = false;
static bool s_downHeld = false;
static unsigned long s_holdStartTime = 0;
static unsigned long s_lastStepTime = 0;

void handleHoldNavigation(bool upHeld, bool downHeld, void (*onStep)(bool isUp), uint16_t stepIntervalMs) {
    static bool wasUp = false;
    static bool wasDown = false;

    unsigned long now = millis();

    // Обнаружение начала удержания
    if (upHeld && !wasUp) {
        s_holdStartTime = now;
        s_upHeld = true;
        s_downHeld = false;
    } else if (downHeld && !wasDown) {
        s_holdStartTime = now;
        s_downHeld = true;
        s_upHeld = false;
    }

    // Сброс при отпускании
    if (!upHeld && !downHeld) {
        s_upHeld = false;
        s_downHeld = false;
        s_lastStepTime = 0;
    }

    // Автопрокрутка
    if ((s_upHeld || s_downHeld) && (now - s_holdStartTime) >= HOLD_THRESHOLD_MS) {
        if (now - s_lastStepTime >= stepIntervalMs) {
            s_lastStepTime = now;
            if (s_upHeld) onStep(true);
            if (s_downHeld) onStep(false);
        }
    }

    wasUp = upHeld;
    wasDown = downHeld;
}
