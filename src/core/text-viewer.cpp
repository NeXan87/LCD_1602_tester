#include "text-viewer.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

void drawTextViewer(TextViewer* viewer, uint8_t lcdRows) {
    if (viewer->scrollOffset < 0) viewer->scrollOffset = 0;
    if (viewer->scrollOffset > (int8_t)(viewer->lineCount - lcdRows)) {
        viewer->scrollOffset = (viewer->lineCount > lcdRows) ? (viewer->lineCount - lcdRows) : 0;
    }

    for (uint8_t row = 0; row < lcdRows && (viewer->scrollOffset + row) < viewer->lineCount; ++row) {
        setCursorLCD(0, row);
        printLCD(viewer->lines[viewer->scrollOffset + row]);
    }

    drawScrollIndicators(viewer->lineCount, lcdRows, viewer->scrollOffset);
}

bool textViewMoveUp(TextViewer* viewer, uint8_t lcdRows) {
    if (viewer->scrollOffset > 0) {
        viewer->scrollOffset--;
        return true;
    }
    return false;
}

bool textViewMoveDown(TextViewer* viewer, uint8_t lcdRows) {
    if (viewer->scrollOffset < (int8_t)(viewer->lineCount - lcdRows)) {
        viewer->scrollOffset++;
        return true;
    }
    return false;
}

static bool defaultStepCallback(bool isUp, void* userData) {
    TextViewer* viewer = (TextViewer*)userData;
    if (isUp) return textViewMoveUp(viewer, LCD_ROWS);
    else return textViewMoveDown(viewer, LCD_ROWS);
}

ScreenId updateTextViewer(
    TextViewer* viewer,
    ScreenId exitScreen,
    TextViewStepCallback stepCallback,
    void* userData) {
    if (!viewer->initialized) {
        viewer->scrollOffset = 0;
        viewer->initialized = true;

        if (viewer->initFunc) {
            viewer->initFunc();
        }

        drawTextViewer(viewer, LCD_ROWS);
    }

    bool changed = false;

    if (clickUpButton()) {
        changed = textViewMoveUp(viewer, LCD_ROWS);
    }
    if (clickDownButton()) {
        changed = textViewMoveDown(viewer, LCD_ROWS);
    }

    bool holdChanged = handleHoldNavigation(isUpButtonPressed(), isDownButtonPressed(), defaultStepCallback, viewer, STEP_INTERVAL_FAST_MS);

    if (changed || holdChanged) {
        clearLCD();
        drawTextViewer(viewer, LCD_ROWS);
    }

    if (clickLeftButton()) {
        clearLCD();
        viewer->initialized = false;
        return exitScreen;
    }

    return viewer->screenId;
}
