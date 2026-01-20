#include "screens/screen-diagnostics.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static const char* const DIAG_MENU_ITEMS[] = {
    "Buttons",
};
static const uint8_t DIAG_MENU_COUNT = sizeof(DIAG_MENU_ITEMS) / sizeof(DIAG_MENU_ITEMS[0]);
static int menuSelectedIndex = 0;

static void drawDiagMenu() {
    drawSubMenu("Diagnostics", DIAG_MENU_ITEMS[menuSelectedIndex], true);
}

void initScreenDiagnostics() {
    menuSelectedIndex = 0;
    drawDiagMenu();
}

ScreenId updateScreenDiagnostics() {
    if (updateMenuIndex(&menuSelectedIndex, DIAG_MENU_COUNT)) {
        drawDiagMenu();
    }

    if (clickSelectButton()) {
        clearLCD();

        if (menuSelectedIndex == 0) {
            return SCREEN_BUTTONS_TEST;
        }
    }

    if (clickLeftButton()) {
        clearLCD();
        return SCREEN_LIST;
    }

    return SCREEN_DIAGNOSTICS;
}
