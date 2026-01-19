#include <Arduino.h>
#include <EEPROM.h>

#include "config.h"
#include "core/screen-manager.h"
#include "drivers/backlight.h"
#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "screens/screen-list.h"
#include "screens/screen-settings.h"
#include "storage/eeprom2.h"

void setup() {
    eepromInit();
    backlightInit();
    initLCD();
    screenListRedraw();
    Serial.begin(115200);
}

void loop() {
    updateScreenManager();
    updateButtons();
}
