#include "screen-list.h"

#include <AnalogMultiButton.h>

#include "buttons.h"
#include "lcd-custom-chars.h"
#include "lcd-driver.h"

static uint8_t selectedIndex = 0;
static int topIndex = 0;
static const char* const MENU_ITEMS[] = {
    "IR test",
    "Encoder test",
    "Settings",
    "Diagnostics",
    "About",
};
static const uint8_t MENU_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

static void adjustTopIndex() {
    if (selectedIndex < topIndex) {
        topIndex = selectedIndex;
        clearLCD();
    } else if (selectedIndex >= topIndex + LCD_ROWS) {
        topIndex = selectedIndex - (LCD_ROWS - 1);
        clearLCD();
    }
    // Гарантируем, что не вышли за границы
    if (topIndex > (int)(MENU_COUNT - LCD_ROWS)) {
        topIndex = (MENU_COUNT >= LCD_ROWS) ? (int)(MENU_COUNT - LCD_ROWS) : 0;
    }
}

static void drawMenu() {
    adjustTopIndex();

    for (uint8_t row = 0; row < LCD_ROWS && (topIndex + row) < MENU_COUNT; ++row) {
        int itemIndex = topIndex + row;

        setCursorLCD(0, row);
        if (itemIndex == selectedIndex) {
            writeCharLCD(126);
        } else {
            printLCD(" ");
        }
        printLCD(MENU_ITEMS[itemIndex]);
    }

    if (MENU_COUNT > LCD_ROWS) {
        if (topIndex > 0) {
            setCursorLCD(LCD_COLS - 1, 0);
            writeCharLCD(LCD_CHAR_ARROW_UP);
        }
        if (topIndex < (int)(MENU_COUNT - LCD_ROWS)) {
            setCursorLCD(LCD_COLS - 1, LCD_ROWS - 1);
            writeCharLCD(LCD_CHAR_ARROW_DOWN);
        }
    }
}

static ScreenId getSelectedScreen() {
    switch (selectedIndex) {
        case 0: return SCREEN_IR_TEST;
        case 1: return SCREEN_ENCODER_TEST;
        case 2: return SCREEN_SETTINGS;
        case 3: return SCREEN_DIAGNOSTICS;
        case 4: return SCREEN_ABOUT;
    }
    return SCREEN_LIST;
}

void screenListRedraw() {
    drawMenu();
}

ScreenId screenListUpdate() {
    if (clickUpButton()) {
        if (selectedIndex > 0) {
            selectedIndex--;
            drawMenu();
        }
    }

    if (clickDownButton()) {
        if (selectedIndex < MENU_COUNT - 1) {
            selectedIndex++;
            drawMenu();
        }
    }

    if (clickSelectButton()) {
        ScreenId next = getSelectedScreen();
        selectedIndex = 0;
        topIndex = 0;
        clearLCD();
        return next;
    }

    return SCREEN_LIST;
}
