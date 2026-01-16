#include "lcd-driver.h"

#include <LiquidCrystal.h>
#include <stdarg.h>

#include "lcd-custom-chars.h"
#include "screen-about.h"

static LiquidCrystal lcd(
    LCD_RS_PIN,
    LCD_EN_PIN,
    LCD_D4_PIN,
    LCD_D5_PIN,
    LCD_D6_PIN,
    LCD_D7_PIN);

static char lcdPrintfBuffer[32];

void clearLCD() {
    lcd.clear();
}

void setCursorLCD(uint8_t col, uint8_t row) {
    if (col < LCD_COLS && row < LCD_ROWS) {
        lcd.setCursor(col, row);
    }
}

void writeCharLCD(uint8_t c) {
    lcd.write(c);
}

void printLCD(const char* str) {
    lcd.print(str);
}

void printIntLCD(long value) {
    lcd.print(value);
}

void printfLCD(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int written = vsnprintf(lcdPrintfBuffer, sizeof(lcdPrintfBuffer), format, args);
    va_end(args);

    if (written > 0) {
        lcd.print(lcdPrintfBuffer);
    }
}

void initCharsAboutScreen() {
    lcd.createChar(LCD_CHAR_ARROW_UP, (uint8_t*)arrowUp);
    lcd.createChar(LCD_CHAR_ARROW_DOWN, (uint8_t*)arrowDown);
}

void initLCD() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    setCursorLCD(0, 0);
    printLCD(TITLE);
    setCursorLCD(0, 1);
    printLCD("Starting...");
    delay(DELAY_STARTUP_MS);
    clearLCD();
}
