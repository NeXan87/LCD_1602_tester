#include "screen-battery-edit.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static bool currentEnabled;
static bool originalEnabled;

static void drawEditor() {
    clearLCD();
    setCursorLCD(0, 0);
    printLCD("Battery support:");
    setCursorLCD(0, 1);
    printLCD(currentEnabled ? "Enabled" : "Disabled");
}

static bool onNavigateStep(bool isUp) {
    currentEnabled = !currentEnabled;
    drawEditor();
    return true;
}

void initScreenBatteryEdit() {
    originalEnabled = getBatteryEnabledEeprom();
    currentEnabled = originalEnabled;
    drawEditor();
}

ScreenId updateScreenBatteryEdit() {
    if (clickUpButton() || clickDownButton()) {
        currentEnabled = !currentEnabled;
        drawEditor();
    }

    handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), onNavigateStep, STEP_INTERVAL_FAST_MS);

    if (clickLeftButton()) {
        // Отменить изменения
        setBatteryEnabledEeprom(originalEnabled);
        return SCREEN_SETTINGS;
    }

    if (clickSelectButton()) {
        // Сохранить изменения
        setBatteryEnabledEeprom(currentEnabled);
        return SCREEN_SETTINGS;
    }

    return SCREEN_BATTERY_EDIT;
}