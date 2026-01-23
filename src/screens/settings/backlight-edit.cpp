#include "backlight-edit.h"

#include "core/param-editor.h"
#include "drivers/backlight.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/lcd-helpers.h"

static void getBrightness(void* out) {
    *(int*)out = getBacklightPercentEeprom();
}

static void saveBrightness(const void* value) {
    setBacklightPercentEeprom(*(const int*)value);
}

static void applyBrightness(const void* value) {
    setBacklightPercentSmooth(*(const int*)value);
}

static bool stepBrightness(void* value, bool isUp, const void* ctx) {
    int* val = (int*)value;
    int step = *(const int*)ctx;
    if (isUp) {
        if (*val <= 90) {
            *val += step;
            applyBrightness(val);
            return true;
        }
    } else {
        if (*val >= 10) {
            *val -= step;
            applyBrightness(val);
            return true;
        }
    }
    return false;
}

static void drawBrightness(const void* value, const char* title) {
    drawNumericEditor(title, *(const int*)value, NUM_WIDTH_3, "%");
}

static int BRIGHTNESS_STEP = 10;
static int g_currentBrightness;
static int g_originalBrightness;

static ParamEditor g_brightnessEditor = {
    .title = "Bridge",
    .screenId = SCREEN_BACKLIGHT_EDIT,
    .exitScreen = SCREEN_SETTINGS,
    .currentValue = &g_currentBrightness,
    .originalValue = &g_originalBrightness,
    .valueSize = sizeof(int),
    .getFromStorage = getBrightness,
    .saveToStorage = saveBrightness,
    .stepHandler = stepBrightness,
    .drawHandler = drawBrightness,
    .applyFunc = applyBrightness,
    .initFunc = initArrowsLCD,
    .stepContext = &BRIGHTNESS_STEP,
    .isInitialized = false,
};

ScreenId updateScreenBacklightEdit() {
    return updateParamEditor(&g_brightnessEditor);
}
