#include "screen-battery-edit.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static IsEnableId currentEnabled;
static IsEnableId originalEnabled;

void initScreenBatteryEdit() {
    originalEnabled = getBatteryEnabledEeprom();
    currentEnabled = originalEnabled;
    drawOnOff(currentEnabled, "Battery support:");
}

ScreenId updateScreenBatteryEdit() {
    if (clickUpButton() || clickDownButton()) {
        currentEnabled = currentEnabled ? OFF : ON;
        drawOnOff(currentEnabled);
    }

    if (clickLeftButton()) {
        setBatteryEnabledEeprom(originalEnabled);
        clearLCD();
        return SCREEN_SETTINGS;
    }

    if (clickSelectButton()) {
        setBatteryEnabledEeprom(currentEnabled);
        clearLCD();
        return SCREEN_SETTINGS;
    }

    return SCREEN_BATTERY_EDIT;
}
