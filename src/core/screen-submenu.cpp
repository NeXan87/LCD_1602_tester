#include "screen-submenu.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

ScreenId updateSubMenu(ScreenSubMenu* menu, SubMenuScreenMapper mapper) {
    if (!menu->isInitialized) {
        menu->selectedIndex = 0;

        if (menu->initFunc != nullptr) {
            menu->initFunc();
        }

        drawSubMenu(menu->title, menu->items[menu->selectedIndex], true);
        menu->isInitialized = true;
    }

    if (updateMenuIndex(&menu->selectedIndex, menu->count)) {
        drawSubMenu(menu->title, menu->items[menu->selectedIndex], true);
    }

    if (clickSelectButton()) {
        clearLCD();
        menu->isInitialized = false;
        return mapper(menu->selectedIndex);
    }

    if (clickLeftButton()) {
        clearLCD();
        menu->isInitialized = false;
        return SCREEN_LIST;
    }

    return menu->screenId;
}
