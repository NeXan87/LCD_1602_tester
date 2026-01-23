#include "text-viewer.h"

#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "utils/hold-navigate.h"
#include "utils/lcd-helpers.h"

void drawTextViewer(TextViewer* viewer) {
    const int16_t maxScrollOffset = (viewer->lineCount > LCD_ROWS) ? (int16_t)(viewer->lineCount - LCD_ROWS) : 0;

    if (viewer->scrollOffset < 0) {
        viewer->scrollOffset = 0;
    } else if (viewer->scrollOffset > maxScrollOffset) {
        viewer->scrollOffset = maxScrollOffset;
    }

    for (uint8_t row = 0; row < LCD_ROWS && (viewer->scrollOffset + row) < viewer->lineCount; ++row) {
        setCursorLCD(0, row);
        printLCD(viewer->lines[viewer->scrollOffset + row]);
    }

    drawScrollIndicators(viewer->lineCount, LCD_ROWS, viewer->scrollOffset);
}

bool isTextViewMoveUp(TextViewer* viewer) {
    if (viewer->scrollOffset > 0) {
        viewer->scrollOffset--;
        return true;
    }
    return false;
}

bool isTextViewMoveDown(TextViewer* viewer) {
    if (viewer->scrollOffset < (int8_t)(viewer->lineCount - LCD_ROWS)) {
        viewer->scrollOffset++;
        return true;
    }
    return false;
}

static bool isDefaultStepCallback(bool isUp, void* userData) {
    TextViewer* viewer = (TextViewer*)userData;
    if (isUp) return isTextViewMoveUp(viewer);
    else return isTextViewMoveDown(viewer);
}

ScreenId updateTextViewer(TextViewer* viewer, ScreenId exitScreen) {
    if (!viewer->isInitialized) {
        viewer->scrollOffset = 0;
        viewer->isInitialized = true;

        if (viewer->initFunc) {
            viewer->initFunc();
        }

        drawTextViewer(viewer);
    }

    bool isChanged = false;

    if (clickUpButton()) {
        isChanged = isTextViewMoveUp(viewer);
    }

    if (clickDownButton()) {
        isChanged = isTextViewMoveDown(viewer);
    }

    bool isHoldChanged = handleHoldNavigation(isDefaultStepCallback, viewer, STEP_INTERVAL_FAST_MS);

    if (isChanged || isHoldChanged) {
        clearLCD();
        drawTextViewer(viewer);
    }

    if (clickLeftButton()) {
        clearLCD();
        viewer->isInitialized = false;
        return exitScreen;
    }

    return viewer->screenId;
}
