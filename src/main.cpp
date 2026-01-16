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
    } else if (clickUpButton()) {
        setDataLcd("Up      ADC:", x);
    } else if (clickDownButton()) {
        setDataLcd("Down    ADC:", x);
    } else if (clickLeftButton()) {
        setDataLcd("Left    ADC:", x);
    } else if (clickSelectButton()) {
        setDataLcd("Select  ADC:", x);
    }
}
