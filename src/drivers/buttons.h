#pragma once

// Идентификаторы кнопок (enum для читаемости)
enum ButtonId {
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_SELECT
};

void updateButtons();
bool clickRightButton();
bool clickUpButton();
bool clickDownButton();
bool clickLeftButton();
bool clickSelectButton();
bool isUpButtonPressed();
bool isDownButtonPressed();
bool isRightButtonPressed();
bool isLeftButtonPressed();
bool isSelectButtonPressed();
bool isLeftButtonHeld();
bool isSelectButtonHeld();
