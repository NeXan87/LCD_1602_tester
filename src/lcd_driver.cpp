#include "lcd_driver.h"

#include <LiquidCrystal.h>
#include <stdarg.h>

static LiquidCrystal lcd(
    LCD_RS_PIN,
    LCD_EN_PIN,
    LCD_D4_PIN,
    LCD_D5_PIN,
    LCD_D6_PIN,
    LCD_D7_PIN);

static char lcdPrintfBuffer[32];

void lcdClear() {
    lcd.clear();
}

void lcdSetCursor(uint8_t col, uint8_t row) {
    if (col < LCD_COLS && row < LCD_ROWS) {
        lcd.setCursor(col, row);
    }
}

void lcdPrint(const char* str) {
    lcd.print(str);
}

void lcdPrintInt(long value) {
    lcd.print(value);
}

void lcdPrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int written = vsnprintf(lcdPrintfBuffer, sizeof(lcdPrintfBuffer), format, args);
    va_end(args);

    if (written > 0) {
        lcd.print(lcdPrintfBuffer);
    }
}

void lcdInit() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcdSetCursor(0, 0);
    lcdPrint("LCD1602 KEY TEST");
    lcdSetCursor(0, 1);
    lcdPrint("Press on Buttons");
}
