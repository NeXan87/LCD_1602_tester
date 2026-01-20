#include "screen-battery-edit.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static uint8_t currentEnabled;
static uint8_t originalEnabled;

static void drawEditor() {
    clearLCD();
    setCursorLCD(0, 0);
    printLCD("Battery support:");
    setCursorLCD(0, 1);
    printLCD(currentEnabled ? "Enabled" : "Disabled");
}

void initScreenBatteryEdit() {
    originalEnabled = getBatteryEnabledEeprom();
    Serial.println(getBatteryEnabledEeprom());
    currentEnabled = originalEnabled;
    drawEditor();
}

ScreenId updateScreenBatteryEdit() {
    if (clickUpButton() || clickDownButton()) {
        currentEnabled = currentEnabled ? 0 : 1;
        drawEditor();
    }

    if (clickLeftButton()) {
        setBatteryEnabledEeprom(originalEnabled);
        return SCREEN_SETTINGS;
    }

    if (clickSelectButton()) {
        setBatteryEnabledEeprom(currentEnabled);
        return SCREEN_SETTINGS;
    }

    return SCREEN_BATTERY_EDIT;
}
