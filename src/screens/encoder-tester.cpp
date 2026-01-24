// screens/screen-encoder-tester.cpp
#include "encoder-tester.h"

#include "core/submenu.h"
#include "drivers/lcd-driver.h"
#include "screens/encoder-tester/ttl.h"
#include "utils/helpers.h"

static const char* const ENCODER_ITEMS[] = {
    "TTL",
};

static ScreenId encoderItemToScreen(int index) {
    switch (index) {
        case 0: return SCREEN_ENCODER_TEST_TTL;
        default: return SCREEN_ENCODER_TEST;
    }
}

static ScreenSubMenu g_encoderMenu = {
    .title = "Encoder tester",
    .items = ENCODER_ITEMS,
    .count = COUNT_OF(ENCODER_ITEMS),
    .selectedIndex = 0,
    .screenId = SCREEN_ENCODER_TEST,
    .exitScreen = SCREEN_MAIN_MENU,
    .isInitialized = false,
    .initFunc = initArrowsLCD,
    .mapper = encoderItemToScreen,
};

ScreenId updateScreenEncoderTest(void) {
    return updateSubMenu(&g_encoderMenu);
}
