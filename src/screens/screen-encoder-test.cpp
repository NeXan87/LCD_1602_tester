#include "screens/screen-encoder-test.h"

#include "config.h"
#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"
#include "utils/lcd-helpers.h"

static const char* const ENCODER_ITEMS[] = {
    "TTL Encoder",
};
static const uint8_t ENCODER_COUNT = sizeof(ENCODER_ITEMS) / sizeof(ENCODER_ITEMS[0]);
static int menuSelectedIndex = 0;

static void drawEncoderMenu() {
    drawSubMenu("Encoder tester", ENCODER_ITEMS[menuSelectedIndex], true);
}

void initScreenEncoderTest() {
    menuSelectedIndex = 0;
    drawEncoderMenu();
}

ScreenId updateScreenEncoderTest() {
    if (updateMenuIndex(&menuSelectedIndex, ENCODER_COUNT)) {
        drawEncoderMenu();
    }

    if (clickSelectButton()) {
        clearLCD();

        if (menuSelectedIndex == 0) {
            return SCREEN_ENCODER_TEST_TTL;  // Нужно добавить этот ID
        }
    }

    if (clickLeftButton()) {
        clearLCD();
        return SCREEN_LIST;
    }

    return SCREEN_ENCODER_TEST;
}
