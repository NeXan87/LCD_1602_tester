#include "lcd-helpers.h"

#include "lcd-custom-chars.h"
#include "lcd-driver.h"

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
        setCursorLCD(LCD_COLS - 1, visibleRows - (uint8_t)1);
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

void drawSubMenu(const char* title, const char* itemText, bool isSelected, uint8_t row = 1) {
    clearLCD();
    setCursorLCD(0, 0);
    printLCD(title);

    drawMenuItem(0, row, itemText, isSelected);
}
