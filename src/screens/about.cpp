#include "about.h"

#include "config.h"
#include "core/text-viewer.h"
#include "drivers/lcd-driver.h"
#include "utils/helpers.h"

static const char* const ABOUT_LINES[] = {
    TITLE,
    GITHUB_URL,
    AUTHOR_NAME,
    LICENSE_TYPE,
    YEAR_RELEASE,
};

static TextViewer g_aboutViewer = {
    .lines = ABOUT_LINES,
    .lineCount = COUNT_OF(ABOUT_LINES),
    .scrollOffset = 0,
    .screenId = SCREEN_ABOUT,
    .isInitialized = false,
    .initFunc = initArrowsLCD,
};

ScreenId updateAboutScreen() {
    return updateTextViewer(&g_aboutViewer, SCREEN_MAIN_MENU);
}
