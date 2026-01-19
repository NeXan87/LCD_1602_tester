#include "screen-backlight-edit.h"

#include "drivers/backlight.h"
#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/lcd-helpers.h"

static int currentBrightness;
static int originalBrightness;

static void drawEditor() {
    drawNumericEditor("Bridge", currentBrightness, 3, "%");
}

void initScreenBacklightEdit() {
    originalBrightness = eepromGetBacklightPercent();
    currentBrightness = originalBrightness;
    drawEditor();
}

bool updateScreenBacklightEdit() {
    if (clickDownButton()) {
        if (currentBrightness >= 10) {
            currentBrightness -= 10;
            setPercentBacklight(currentBrightness);
            drawEditor();
        }
    }
    if (clickUpButton()) {
        if (currentBrightness <= 90) {
            currentBrightness += 10;
            setPercentBacklight(currentBrightness);
            drawEditor();
        }
    }
    if (clickLeftButton()) {
        setPercentBacklight(originalBrightness);
        return true;
    }
    if (clickSelectButton()) {
        eepromSetBacklightPercent(currentBrightness);
        eepromSaveSettings();
        return true;
    }
    return false;
}
