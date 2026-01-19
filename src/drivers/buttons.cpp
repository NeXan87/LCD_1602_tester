#include "drivers/buttons.h"

#include <AnalogMultiButton.h>

#include "config.h"

static constexpr int BUTTONS_VALUES[BUTTONS_TOTAL] = {BUTTON_RIGHT_ADC, BUTTON_UP_ADC, BUTTON_DOWN_ADC, BUTTON_LEFT_ADC, BUTTON_SELECT_ADC};
static AnalogMultiButton buttons(BUTTONS_PIN, BUTTONS_TOTAL, BUTTONS_VALUES, BUTTONS_DEBOUNCE_MS);

void updateButtons() {
    buttons.update();
}

bool clickRightButton() {
    return buttons.onPress(BUTTON_RIGHT);
}

bool clickUpButton() {
    return buttons.onPress(BUTTON_UP);
}

bool clickDownButton() {
    return buttons.onPress(BUTTON_DOWN);
}

bool clickLeftButton() {
    return buttons.onPress(BUTTON_LEFT);
}

bool clickSelectButton() {
    return buttons.onPress(BUTTON_SELECT);
}

bool isUpButtonPressed() {
    return buttons.isPressed(BUTTON_UP);
}

bool isDownButtonPressed() {
    return buttons.isPressed(BUTTON_DOWN);
}

bool isLeftButtonHeld() {
    return buttons.isPressedAfter(BUTTON_LEFT, BUTTONS_HOLD_MS);
}

bool isSelectButtonHeld() {
    return buttons.isPressedAfter(BUTTON_SELECT, BUTTONS_HOLD_MS);
}
