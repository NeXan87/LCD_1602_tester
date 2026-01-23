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
    printfLCD("%-15s", text);
}

void drawOnOff(IsEnableId isEnabled, const char* title) {
    if (title) {
        setCursorLCD(0, 0);
        printLCD(title);
    }

    setCursorLCD(0, 1);
    printfLCD("%-3s", isEnabled ? "ON" : "OFF");
}

void drawNumericEditor(const char* title, int value, NumericWidth width, const char* suffix) {
    if (title) {
        setCursorLCD(0, 0);
        printLCD(title);
    }

    setCursorLCD(0, 1);

    if (width == NUM_WIDTH_2) {
        printfLCD("%2d%s", value, suffix);
    } else if (width == NUM_WIDTH_3) {
        printfLCD("%3d%s", value, suffix);
    } else if (width == NUM_WIDTH_4) {
        printfLCD("%4d%s", value, suffix);
    } else {
        printfLCD("%d%s", value, suffix);
    }
}
