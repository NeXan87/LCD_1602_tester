#include <Arduino.h>
#include <EEPROM.h>

#include "config.h"
#include "core/screen-manager.h"
#include "drivers/backlight.h"
#include "drivers/battery.h"
#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "screens/main-list.h"
#include "screens/settings.h"
#include "storage/eeprom2.h"

void setup() {
    Serial.begin(115200);
    initEeprom();
    initBacklight();
    initLCD();
    initBattery();
    redrawScreenList();
}

void loop() {
    updateButtons();
    updateBattery();
    updateBacklight();
    updateScreenManager();
}
