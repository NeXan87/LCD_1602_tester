#include "screens/screen-about.h"

#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static const char* const ABOUT_LINES[] = {
    TITLE,
    GITHUB_URL,
    AUTHOR_NAME,
    LICENSE_TYPE,
    YEAR_RELEASE,
};
static const uint8_t ABOUT_LINE_COUNT = sizeof(ABOUT_LINES) / sizeof(ABOUT_LINES[0]);
static int8_t scrollOffset = 0;

static void drawAboutScreen() {
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > (int8_t)(ABOUT_LINE_COUNT - LCD_ROWS)) {
        scrollOffset = (ABOUT_LINE_COUNT > LCD_ROWS) ? (ABOUT_LINE_COUNT - LCD_ROWS) : 0;
    }

    for (uint8_t row = 0; row < LCD_ROWS && (scrollOffset + row) < ABOUT_LINE_COUNT; ++row) {
        setCursorLCD(0, row);
        printLCD(ABOUT_LINES[scrollOffset + row]);
    }

    drawScrollIndicators(ABOUT_LINE_COUNT, LCD_ROWS, scrollOffset);
}

void initAboutScreen() {
    scrollOffset = 0;
    drawAboutScreen();
}

static bool moveDownStep() {
    if (scrollOffset < (int8_t)(ABOUT_LINE_COUNT - LCD_ROWS)) {
        scrollOffset++;
        return true;
    }
    return false;
}

static bool moveUpStep() {
    if (scrollOffset > 0) {
        scrollOffset--;
        return true;
    }
    return false;
}

static bool onNavigateStep(bool isUp) {
    if (isUp) {
        return moveUpStep();
    } else {
        return moveDownStep();
    }
}

ScreenId updateAboutScreen() {
    bool changed = false;

    if (clickUpButton()) {
        changed = moveUpStep();
    }

    if (clickDownButton()) {
        changed = moveDownStep();
    }

    bool holdChanged = handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), onNavigateStep, STEP_INTERVAL_FAST_MS);

    if (changed || holdChanged) {
        clearLCD();
        drawAboutScreen();
    }

    if (clickLeftButton()) {
        clearLCD();
        return SCREEN_LIST;
    }

    return SCREEN_ABOUT;
}
