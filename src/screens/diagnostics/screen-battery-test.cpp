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

    float voltage = getBatteryVoltage();
    int percent = getBatteryPercent();
    int adc = getBatteryAdcValue();

    char vbuf[6];
    dtostrf(voltage, 4, 1, vbuf);

    setCursorLCD(0, 0);
    printfLCD("Vbat=%sV %3d%%", vbuf, percent);
    setCursorLCD(0, 1);
    printfLCD("ADC: %-4d", adc);
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

    if (clickLeftButton()) {
        isInitBatteryTest = false;
        lastUpdate = 0;
        return SCREEN_DIAGNOSTICS;
    }

    return SCREEN_BATTERY_TEST;
}
