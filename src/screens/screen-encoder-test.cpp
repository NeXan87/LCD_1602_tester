// screens/screen-encoder-test.cpp
#include "screen-encoder-test.h"

#include "core/screen-submenu.h"
#include "drivers/lcd-driver.h"
#include "screens/screen-encoder-ttl-test.h"
#include "utils/helpers.h"

static const char* const ENCODER_ITEMS[] = {
    "TTL Encoder",
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
    .initialized = false,
    .initFunc = initArrowsLCD,
};

ScreenId updateScreenEncoderTest(void) {
    return updateSubMenu(&g_encoderMenu, encoderItemToScreen);
}
