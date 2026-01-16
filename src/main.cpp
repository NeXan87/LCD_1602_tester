#include <Arduino.h>

#include "buttons.h"
#include "config.h"
#include "lcd_driver.h"

void setup() {
    lcdInit();
    Serial.begin(115200);
}

void setDataLcd(const char* str, int x) {
    lcdSetCursor(0, 1);
    lcdPrint("                ");
    lcdSetCursor(0, 1);
    lcdPrint(str);
    lcdSetCursor(12, 1);
    lcdPrintInt(x);
}

void loop() {
    int x = analogRead(BUTTONS_PIN);
    updateButtons();

    if (clickRightButton()) {
        setDataLcd("Right   ADC:", x);
        Serial.print("Value A0 `Right` is  :");
        Serial.println(x, DEC);
    } else if (clickUpButton()) {
        setDataLcd("Up      ADC:", x);
        Serial.print("Value A0 `UP`’ is  :");
        Serial.println(x, DEC);
    } else if (clickDownButton()) {
        setDataLcd("Down    ADC:", x);
        Serial.print("Value A0‘`Down` is  :");
        Serial.println(x, DEC);
    } else if (clickLeftButton()) {
        setDataLcd("Left    ADC:", x);
        Serial.print("Value A0 `Left` is  :");
        Serial.println(x, DEC);
    } else if (clickSelectButton()) {
        setDataLcd("Select  ADC:", x);
        Serial.print("Value A0 `Select` is  :");
        Serial.println(x, DEC);
    }
}
