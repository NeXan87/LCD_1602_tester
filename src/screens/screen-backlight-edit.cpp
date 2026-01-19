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
    drawNumericEditor("Bridge", currentBrightness, NUM_WIDTH_3, "%");
}

void initScreenBacklightEdit() {
    originalBrightness = getBacklightPercentEeprom();
    currentBrightness = originalBrightness;
    drawEditor();
}

bool updateScreenBacklightEdit() {
    if (clickDownButton()) {
        if (currentBrightness >= 10) {
            currentBrightness -= 10;
            setBacklightPercent(currentBrightness);
            drawEditor();
        }
    }
    if (clickUpButton()) {
        if (currentBrightness <= 90) {
            currentBrightness += 10;
            setBacklightPercent(currentBrightness);
            drawEditor();
        }
    }
    if (clickLeftButton()) {
        setBacklightPercent(originalBrightness);
        return true;
    }
    if (clickSelectButton()) {
        setBacklightPercentEeprom(currentBrightness);
        return true;
    }
    return false;
}
