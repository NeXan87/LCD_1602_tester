#include "screens/screen-list.h"

#include "core/screen-manager.h"
#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

static int selectedIndex = 0;
static int topIndex = 0;
static const char* const MENU_ITEMS[] = {
    "IR tester",
    "Enc tester",
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
        drawMenuItem(0, row, MENU_ITEMS[itemIndex], itemIndex == selectedIndex);
    }

    drawScrollIndicators(MENU_COUNT, LCD_ROWS, topIndex);
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

void redrawScreenList() {
    drawMenu();
}

static bool onNavigateStep(bool isUp, void* userData) {
    (void)userData;

    if (isUp) {
        if (selectedIndex > 0) {
            selectedIndex--;
            return true;
        }
    } else {
        if (selectedIndex < MENU_COUNT - 1) {
            selectedIndex++;
            return true;
        }
    }
    return false;
}

ScreenId updateScreenList() {
    bool changed = updateMenuIndex(&selectedIndex, MENU_COUNT);

    bool holdChanged = handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), onNavigateStep, nullptr);

    if (changed || holdChanged) {
        drawMenu();
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
