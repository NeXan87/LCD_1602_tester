#include "screen-about.h"

#include "buttons.h"
#include "lcd-custom-chars.h"
#include "lcd-driver.h"

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

    if (ABOUT_LINE_COUNT > LCD_ROWS) {
        if (scrollOffset > 0) {
            setCursorLCD(LCD_COLS - 1, 0);
            writeCharLCD(LCD_CHAR_ARROW_UP);
        }
        if (scrollOffset < (int8_t)(ABOUT_LINE_COUNT - LCD_ROWS)) {
            setCursorLCD(LCD_COLS - 1, LCD_ROWS - 1);
            writeCharLCD(LCD_CHAR_ARROW_DOWN);
        }
    }
}

void initAboutScreen() {
    scrollOffset = 0;
    drawAboutScreen();
}

bool updateAboutScreen() {
    bool changed = false;

    if (clickUpButton()) {
        if (scrollOffset > 0) {
            scrollOffset--;
            changed = true;
        }
    }

    if (clickDownButton()) {
        if (scrollOffset < (int8_t)(ABOUT_LINE_COUNT - LCD_ROWS)) {
            scrollOffset++;
            changed = true;
        }
    }

    if (changed) {
        clearLCD();
        drawAboutScreen();
    }

    // Выход по SELECT или LEFT
    if (clickSelectButton() || clickLeftButton()) {
        clearLCD();
        return true;
    }

    return false;
}
