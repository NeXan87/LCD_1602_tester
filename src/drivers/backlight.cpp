#include "backlight.h"

#include "drivers/battery.h"
#include "storage/eeprom2.h"
#include "utils/helpers.h"

static int currentBacklightPercent = MAX_PERCENT;

static bool isFading = false;
static int fadeStartPercent;
static int fadeTargetPercent;
static unsigned long fadeStartTime;
static unsigned long fadeDuration;

void setBacklightPercent(int percent) {
    percent = clamp(percent, MIN_PERCENT, MAX_PERCENT);
    analogWrite(BACKLIGHT_PIN, percentToPwm(percent));
    currentBacklightPercent = percent;
    isFading = false;  // Остановить плавное изменение, если оно было
}

void setBacklightPercentSmooth(int targetPercent, unsigned long durationMs) {
    targetPercent = clamp(targetPercent, MIN_PERCENT, MAX_PERCENT);
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
            current = clamp(current, MIN_PERCENT, MAX_PERCENT);
            analogWrite(BACKLIGHT_PIN, percentToPwm(current));
            currentBacklightPercent = current;
        }
    }

    if (getBatteryEnabledEeprom() && shouldDimBacklight()) {
        int lowBrightness = BATTERY_LOW_BRIGHTNESS;
        if (currentBacklightPercent > lowBrightness) {
            setBacklightPercent(lowBrightness);
        }
    } else if (getBatteryEnabledEeprom() && currentBacklightPercent == BATTERY_LOW_BRIGHTNESS) {
        saveApplyBacklight();
    }
}

void saveApplyBacklight() {
    int saved = getBacklightPercentEeprom();
    setBacklightPercent(saved);
}

void initBacklight() {
    pinMode(BACKLIGHT_PIN, OUTPUT);
    saveApplyBacklight();
}
