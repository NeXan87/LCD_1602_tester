#include "diagnostics.h"

#include "core/submenu.h"
#include "screens/diagnostics/battery.h"
#include "screens/diagnostics/buttons.h"
#include "utils/helpers.h"

static const char* const DIAG_ITEMS[] = {
    "Buttons",
    "Battery",
};

static ScreenId getDiagItemScreen(int index) {
    switch (index) {
        case 0: return SCREEN_BUTTONS_DIAG;
        case 1: return SCREEN_BATTERY_DIAG;
        default: return SCREEN_DIAGNOSTICS;
    }
}

static ScreenSubMenu g_diagMenu = {
    .title = "Diagnostics",
    .items = DIAG_ITEMS,
    .count = COUNT_OF(DIAG_ITEMS),
    .selectedIndex = 0,
    .screenId = SCREEN_DIAGNOSTICS,
    .exitScreen = SCREEN_MAIN_MENU,
    .isInitialized = false,
    .initFunc = nullptr,
    .mapper = getDiagItemScreen,
};

ScreenId updateScreenDiagnostics() {
    return updateSubMenu(&g_diagMenu);
}
