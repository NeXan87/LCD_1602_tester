#include "battery-edit.h"

#include "core/param-editor.h"
#include "storage/eeprom2.h"
#include "utils/lcd-helpers.h"

static void getBatteryEnabled(void* out) {
    *(IsEnableId*)out = getBatteryEnabledEeprom();  // возвращает IsEnableId
}
static void saveBatteryEnabled(const void* value) {
    setBatteryEnabledEeprom(*(const IsEnableId*)value);
}
static bool stepBattery(void* value, bool isUp, const void* ctx) {
    IsEnableId* val = (IsEnableId*)value;
    *val = (*val == ON) ? OFF : ON;  // переключение
    return true;
}
static void drawBattery(const void* value, const char* title) {
    drawOnOff(*(const IsEnableId*)value, title);
}

static IsEnableId g_currentEnabled;
static IsEnableId g_originalEnabled;

static ParamEditor g_batteryEditor = {
    .title = "Battery support:",
    .exitScreen = SCREEN_SETTINGS,
    .currentValue = &g_currentEnabled,
    .originalValue = &g_originalEnabled,
    .valueSize = sizeof(IsEnableId),  // ← важно!
    .getFromStorage = getBatteryEnabled,
    .saveToStorage = saveBatteryEnabled,
    .stepHandler = stepBattery,
    .drawHandler = drawBattery,
    .initFunc = nullptr,
    .stepContext = nullptr,
    .isInitialized = false,
};

ScreenId updateScreenBatteryEdit() {
    return updateParamEditor(&g_batteryEditor);
}
