#include "submenu.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

void drawSubMenu(const char* title, const char* itemText, bool isSelected, uint8_t row = 1) {
    drawMenuItem(0, row, itemText, isSelected);
}

static bool isSubMenuStepCallback(bool isUp, void* userData) {
    ScreenSubMenu* menu = (ScreenSubMenu*)userData;
    if (isUp) {
        if (menu->selectedIndex > 0) {
            menu->selectedIndex--;
            return true;
        }
    } else {
        if (menu->selectedIndex < (int)(menu->count - 1)) {
            menu->selectedIndex++;
            return true;
        }
    }
    return false;
}

ScreenId updateSubMenu(ScreenSubMenu* menu) {
    if (!menu->isInitialized) {
        menu->selectedIndex = 0;
        menu->isInitialized = true;

        if (menu->initFunc != nullptr) {
            menu->initFunc();
        }

        setCursorLCD(0, 0);
        printLCD(menu->title);
        drawSubMenu(menu->title, menu->items[menu->selectedIndex], true);
    }

    bool isChanged = false;

    if (clickUpButton()) {
        if (isSubMenuStepCallback(true, menu)) {
            isChanged = true;
        }
    }
    if (clickDownButton()) {
        if (isSubMenuStepCallback(false, menu)) {
            isChanged = true;
        }
    }

    bool isHoldChanged = handleHoldNavigation(isSubMenuStepCallback, menu);

    if (isChanged || isHoldChanged) {
        drawSubMenu(menu->title, menu->items[menu->selectedIndex], true);
    }

    if (clickSelectButton()) {
        clearLCD();
        menu->isInitialized = false;
        return menu->mapper(menu->selectedIndex);
    }

    if (menu->exitScreen != SCREEN_NONE && clickLeftButton()) {
        clearLCD();
        menu->isInitialized = false;
        return menu->exitScreen;
    }

    return menu->screenId;
}
