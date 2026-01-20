#include "screen-battery-test.h"

#include "drivers/battery.h"
#include "drivers/buttons.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"
#include "utils/lcd-helpers.h"

static bool isInitBatteryTest = false;

static void drawBatteryTest() {
    if (!isInitBatteryTest) {
        clearLCD();
        setCursorLCD(0, 0);
        printLCD("Battery Test:");
        isInitBatteryTest = true;
    }

    if (!getBatteryEnabledEeprom()) {
        setCursorLCD(0, 1);
        printLCD("Battery disabled");
        return;
    }

    updateBattery();

    char line1[17];
    char line2[17];

    float voltage = getBatteryVoltage();
    int percent = getBatteryPercent();
    int adc = getBatteryAdcValue();

    char vbuf[6];
    dtostrf(voltage, 4, 1, vbuf);
    snprintf(line1, sizeof(line1), "V:%sV %d%%", vbuf, percent);
    snprintf(line2, sizeof(line2), "ADC:%d", adc);

    setCursorLCD(0, 1);
    printLCD(line1);
    // Для второй строки, если нужно, но LCD 2 строки, так что перезаписываем
    // Подождем, LCD 16 символов, line1 может быть длинным.
    // Лучше использовать printfLCD или разделить.

    // Перерисуем полностью
    clearLCD();
    setCursorLCD(0, 0);
    printfLCD("Vbat=%sV %d%%", vbuf, percent);
    setCursorLCD(0, 1);
    printfLCD("ADC:%d", adc);
}

void initScreenBatteryTest() {
    drawBatteryTest();
}

ScreenId updateScreenBatteryTest() {
    static uint32_t lastUpdate = 0;
    if (millis() - lastUpdate > BATTERY_TEST_UPDATE_MS) {
        lastUpdate = millis();
        drawBatteryTest();
    }

    if (isLeftButtonHeld()) {
        isInitBatteryTest = false;
        lastUpdate = 0;
        return SCREEN_DIAGNOSTICS;
    }

    return SCREEN_BATTERY_TEST;
}
