#include "diagnostics.h"

#include "core/submenu.h"
#include "screens/diagnostics/battery-test.h"
#include "screens/diagnostics/buttons-test.h"
#include "utils/helpers.h"

static const char* const DIAG_ITEMS[] = {
    "Buttons",
    "Battery",
};

static ScreenSubMenu g_diagMenu = {
    .title = "Diagnostics",
    .items = DIAG_ITEMS,
    .count = COUNT_OF(DIAG_ITEMS),
    .selectedIndex = 0,
    .screenId = SCREEN_DIAGNOSTICS,
    .exitScreen = SCREEN_LIST,
    .isInitialized = false,
    .initFunc = nullptr,
};

static ScreenId getDiagItemScreen(int index) {
    switch (index) {
        case 0: return SCREEN_BUTTONS_TEST;
        case 1: return SCREEN_BATTERY_TEST;
        default: return SCREEN_DIAGNOSTICS;
    }
}

ScreenId updateScreenDiagnostics() {
    return updateSubMenu(&g_diagMenu, getDiagItemScreen);
}
