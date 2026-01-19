#include "backlight.h"

#include "config.h"
#include "storage/eeprom2.h"
#include "utils/helpers.h"

static int g_brightness = 100;

void backlightInit() {
    pinMode(BACKLIGHT_PIN, OUTPUT);
    backlightApplySaved();
}

void backlightSetPercent(int percent) {
    if (percent > 100) percent = 100;
    g_brightness = percent;
    analogWrite(BACKLIGHT_PIN, percentToPwm(g_brightness));
}

int backlightGetPercent() {
    return g_brightness;
}

void backlightApplySaved() {
    int saved = eepromGetBacklightPercent();
    backlightSetPercent(saved);
}
