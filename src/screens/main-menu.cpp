#include "main-menu.h"

#include "core/menu.h"
#include "drivers/lcd-driver.h"
#include "utils/helpers.h"

// Данные меню
static const char* const MENU_ITEMS[] = {
    "IR tester",
    "Enc tester",
    "Settings",
    "Diagnostics",
    "About"};

// Маппинг
static ScreenId menuIndexToScreen(int index) {
    switch (index) {
        case 0: return SCREEN_IR_TEST;
        case 1: return SCREEN_ENCODER_TEST;
        case 2: return SCREEN_SETTINGS;
        case 3: return SCREEN_DIAGNOSTICS;
        case 4: return SCREEN_ABOUT;
        default: return SCREEN_MAIN_MENU;
    }
}

// Состояние меню
static ScreenMenu g_mainMenu = {
    .items = MENU_ITEMS,
    .itemCount = COUNT_OF(MENU_ITEMS),
    .selectedIndex = 0,
    .topIndex = 0,
    .screenId = SCREEN_MAIN_MENU,
    .exitScreen = SCREEN_NONE,
    .isInitialized = false,
    .initFunc = initArrowsLCD,
    .mapper = menuIndexToScreen,
};

void redrawMainMenu() {
    if (g_mainMenu.isInitialized) {
        clearLCD();
        drawMenu(&g_mainMenu, LCD_ROWS);
    }
}

ScreenId updateMainMenu() {
    return updateScreenMenu(&g_mainMenu);
}
