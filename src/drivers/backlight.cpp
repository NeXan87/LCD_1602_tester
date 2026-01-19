#include "backlight.h"

#include "storage/eeprom2.h"
#include "utils/helpers.h"

static int currentBacklightPercent = 100;

static bool isFading = false;
static int fadeStartPercent;
static int fadeTargetPercent;
static unsigned long fadeStartTime;
static unsigned long fadeDuration;

void initBacklight() {
    pinMode(BACKLIGHT_PIN, OUTPUT);
    saveApplyBacklight();
}

void setBacklightPercent(int percent) {
    if (percent > 100) percent = 100;
    if (percent < 0) percent = 0;
    analogWrite(BACKLIGHT_PIN, percentToPwm(percent));
    currentBacklightPercent = percent;
    isFading = false;  // Остановить плавное изменение, если оно было
}

void setBacklightPercentSmooth(int targetPercent, unsigned long durationMs) {
    if (targetPercent > 100) targetPercent = 100;
    if (targetPercent < 0) targetPercent = 0;
    fadeStartPercent = currentBacklightPercent;
    fadeTargetPercent = targetPercent;
    fadeStartTime = millis();
    fadeDuration = durationMs;
    isFading = true;
}

void updateBacklight() {
    if (isFading) {
        unsigned long elapsed = millis() - fadeStartTime;
        if (elapsed >= fadeDuration) {
            setBacklightPercent(fadeTargetPercent);
        } else {
            int diff = fadeTargetPercent - fadeStartPercent;
            float progress = (float)elapsed / (float)fadeDuration;
            int current = fadeStartPercent + (int)(diff * progress);
            if (current < 0) current = 0;
            if (current > 100) current = 100;
            analogWrite(BACKLIGHT_PIN, percentToPwm(current));
        }
    }
}

void saveApplyBacklight() {
    int saved = getBacklightPercentEeprom();
    setBacklightPercent(saved);
}
