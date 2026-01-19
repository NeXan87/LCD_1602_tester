#include "backlight.h"

#include "config.h"
#include "storage/eeprom2.h"
#include "utils/helpers.h"

void initBacklight() {
    pinMode(BACKLIGHT_PIN, OUTPUT);
    saveApplyBacklight();
}

void setBacklightPercent(int percent) {
    if (percent > 100) percent = 100;
    analogWrite(BACKLIGHT_PIN, percentToPwm(percent));
}

void saveApplyBacklight() {
    int saved = getBacklightPercentEeprom();
    setBacklightPercent(saved);
}
