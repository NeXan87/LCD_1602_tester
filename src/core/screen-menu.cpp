#include "screen-menu.h"

#include "config.h"
#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

void drawScreenMenu(ScreenMenu* menu, uint8_t lcdRows) {
    // Ограничиваем topIndex
    if (menu->topIndex < 0) menu->topIndex = 0;
    const int maxTop = (menu->itemCount > lcdRows) ? (int)(menu->itemCount - lcdRows) : 0;
    if (menu->topIndex > maxTop) menu->topIndex = maxTop;

    for (uint8_t row = 0; row < lcdRows && (menu->topIndex + row) < menu->itemCount; ++row) {
        int idx = menu->topIndex + row;
        drawMenuItem(0, row, menu->items[idx], idx == menu->selectedIndex);
    }

    drawScrollIndicators(menu->itemCount, lcdRows, menu->topIndex);
}

bool isScreenMenuMoveUp(ScreenMenu* menu, uint8_t lcdRows) {
    if (menu->selectedIndex > 0) {
        menu->selectedIndex--;
        if (menu->selectedIndex < menu->topIndex) {
            menu->topIndex = menu->selectedIndex;
        }
        return true;
    }
    return false;
}

bool isScreenMenuMoveDown(ScreenMenu* menu, uint8_t lcdRows) {
    if (menu->selectedIndex < (int)(menu->itemCount - 1)) {
        menu->selectedIndex++;
        if (menu->selectedIndex >= menu->topIndex + (int)lcdRows) {
            menu->topIndex = menu->selectedIndex - (lcdRows - 1);
        }
        return true;
    }
    return false;
}

static bool defaultStepCallback(bool isUp, void* userData) {
    ScreenMenu* menu = (ScreenMenu*)userData;
    if (isUp) return isScreenMenuMoveUp(menu, LCD_ROWS);
    else return isScreenMenuMoveDown(menu, LCD_ROWS);
}

ScreenId updateScreenMenu(ScreenMenu* menu, MenuScreenMapper mapper, ScreenId exitScreen) {
    if (!menu->isInitialized) {
        menu->selectedIndex = 0;
        menu->topIndex = 0;
        menu->isInitialized = true;

        if (menu->initFunc != nullptr) {
            menu->initFunc();
        }

        drawScreenMenu(menu, LCD_ROWS);
    }

    bool isChanged = false;

    if (clickUpButton()) {
        isChanged = isScreenMenuMoveUp(menu, LCD_ROWS);
    }
    if (clickDownButton()) {
        isChanged = isScreenMenuMoveDown(menu, LCD_ROWS);
    }

    bool isHoldChanged = handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), defaultStepCallback, menu);

    if (isChanged || isHoldChanged) {
        clearLCD();
        drawScreenMenu(menu, LCD_ROWS);
    }

    if (clickSelectButton()) {
        ScreenId next = mapper(menu->selectedIndex);
        menu->isInitialized = false;
        clearLCD();
        return next;
    }

    if (exitScreen != SCREEN_NONE && clickLeftButton()) {
        menu->isInitialized = false;
        clearLCD();
        return exitScreen;
    }

    return menu->screenId;
}
