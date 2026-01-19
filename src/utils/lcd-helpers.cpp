#include "utils/lcd-helpers.h"

#include "drivers/buttons.h"
#include "drivers/lcd-custom-chars.h"
#include "drivers/lcd-driver.h"

void drawScrollIndicators(uint8_t totalItems, uint8_t visibleRows, int topIndex) {
    if (totalItems <= visibleRows) {
        return;  // прокрутка не нужна
    }

    // Стрелка вверх: есть скрытые пункты сверху
    if (topIndex > 0) {
        setCursorLCD(LCD_COLS - 1, 0);
        writeCharLCD(LCD_CHAR_ARROW_UP);
    }

    // Стрелка вниз: есть скрытые пункты снизу
    if (topIndex < (int)(totalItems - visibleRows)) {
        setCursorLCD(LCD_COLS - 1, (uint8_t)(visibleRows - 1));
        writeCharLCD(LCD_CHAR_ARROW_DOWN);
    }
}

void drawMenuItem(uint8_t col, uint8_t row, const char* text, bool isSelected) {
    setCursorLCD(col, row);
    if (isSelected) {
        writeCharLCD(126);  // → (стрелка вправо из HD44780 ROM)
    } else {
        printLCD(" ");
    }
    printLCD(text);
}

void drawSubMenu(const char* title, const char* itemText, bool isSelected, uint8_t row) {
    clearLCD();
    setCursorLCD(0, 0);
    printLCD(title);

    drawMenuItem(0, row, itemText, isSelected);
}

bool updateMenuIndex(int* currentIndex, uint8_t itemCount) {
    bool isChanged = false;
    if (clickUpButton()) {
        if (*currentIndex > 0) {
            (*currentIndex)--;
            isChanged = true;
        }
    }
    if (clickDownButton()) {
        if (*currentIndex < (int)(itemCount - 1)) {
            (*currentIndex)++;
            isChanged = true;
        }
    }
    return isChanged;
}

void drawNumericEditor(const char* title, int value, NumericWidth width, const char* suffix) {
    char buffer[17];

    if (width == 2) {
        snprintf(buffer, sizeof(buffer), "%s:%2d%s", title, value, suffix);
    } else if (width == 3) {
        snprintf(buffer, sizeof(buffer), "%s:%3d%s", title, value, suffix);
    } else if (width == 4) {
        snprintf(buffer, sizeof(buffer), "%s:%4d%s", title, value, suffix);
    } else {
        snprintf(buffer, sizeof(buffer), "%s:%d%s", title, value, suffix);
    }

    buffer[16] = '\0';

    setCursorLCD(0, 0);
    printLCD(buffer);

    setCursorLCD(0, 1);
    writeCharLCD(LCD_CHAR_ARROW_UP);
    writeCharLCD(' ');
    writeCharLCD(LCD_CHAR_ARROW_DOWN);
    printLCD(" to adjust");
}
