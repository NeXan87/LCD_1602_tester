#include "screen-backlight-edit.h"

#include "drivers/backlight.h"
#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static int currentBrightness;
static int originalBrightness;

static void drawEditor() {
    drawNumericEditor("Bridge", currentBrightness, NUM_WIDTH_3, "%");
}

void initScreenBacklightEdit() {
    originalBrightness = getBacklightPercentEeprom();
    currentBrightness = originalBrightness;
    drawEditor();
}

static void decreaseBacklightByStep() {
    if (currentBrightness >= 10) {
        currentBrightness -= 10;
        setBacklightPercent(currentBrightness);
        drawEditor();
    }
}

static void increaseBacklightByStep() {
    if (currentBrightness <= 90) {
        currentBrightness += 10;
        setBacklightPercent(currentBrightness);
        drawEditor();
    }
}

static bool onNavigateStep(bool isUp, void* userData) {
    (void)userData;

    if (isUp) {
        increaseBacklightByStep();
    } else {
        decreaseBacklightByStep();
    }
    return true;
}

ScreenId updateScreenBacklightEdit() {
    if (clickDownButton()) {
        decreaseBacklightByStep();
    }

    if (clickUpButton()) {
        increaseBacklightByStep();
    }

    handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), onNavigateStep, nullptr, STEP_INTERVAL_FAST_MS);

    if (clickLeftButton()) {
        setBacklightPercentSmooth(originalBrightness);
        return SCREEN_SETTINGS;
    }

    if (clickSelectButton()) {
        setBacklightPercentEeprom(currentBrightness);
        return SCREEN_SETTINGS;
    }

    return SCREEN_BACKLIGHT_EDIT;
}
