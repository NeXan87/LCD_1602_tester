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
    char buffer[17];
    snprintf(buffer, sizeof(buffer), "Bridge:%3d%%", currentBrightness);
    setCursorLCD(0, 0);
    printLCD(buffer);
    setCursorLCD(0, 1);
    writeCharLCD(LCD_CHAR_ARROW_UP);
    writeCharLCD(' ');
    writeCharLCD(LCD_CHAR_ARROW_DOWN);
    printLCD(" to adjust");
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
