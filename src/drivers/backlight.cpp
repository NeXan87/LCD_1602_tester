#include "backlight.h"

#include "config.h"
#include "storage/eeprom2.h"
#include "utils/helpers.h"

void initBacklight() {
    pinMode(BACKLIGHT_PIN, OUTPUT);
    saveApplyBacklight();
}

void setPercentBacklight(int percent) {
    if (percent > 100) percent = 100;
    analogWrite(BACKLIGHT_PIN, percentToPwm(percent));
}

void saveApplyBacklight() {
    int saved = eepromGetBacklightPercent();
    setPercentBacklight(saved);
}
