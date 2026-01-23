#include "battery-edit.h"

#include "core/param-editor.h"
#include "storage/eeprom2.h"
#include "utils/lcd-helpers.h"
#include "utils/param-helpers.h"

static void getBatteryEnabled(void* out) {
    *(IsEnableId*)out = getBatteryEnabledEeprom();
}

static void saveBatteryEnabled(const void* value) {
    setBatteryEnabledEeprom(*(const IsEnableId*)value);
}

static void drawBattery(const void* value, const char* title) {
    drawOnOff(*(const IsEnableId*)value, title);
}

static IsEnableId g_currentEnabled;
static IsEnableId g_originalEnabled;

static ParamEditor g_batteryEditor = {
    .title = "Battery support:",
    .screenId = SCREEN_BATTERY_EDIT,
    .exitScreen = SCREEN_SETTINGS,
    .currentValue = &g_currentEnabled,
    .originalValue = &g_originalEnabled,
    .valueSize = sizeof(IsEnableId),
    .getFromStorage = getBatteryEnabled,
    .saveToStorage = saveBatteryEnabled,
    .stepHandler = toggleIsEnableIdStep,
    .drawHandler = drawBattery,
    .applyFunc = nullptr,
    .initFunc = nullptr,
    .stepContext = nullptr,
    .isInitialized = false,
};

ScreenId updateScreenBatteryEdit() {
    return updateParamEditor(&g_batteryEditor);
}
