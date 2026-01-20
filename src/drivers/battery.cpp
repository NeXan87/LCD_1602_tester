#include "battery.h"

#include <avr/sleep.h>

#include "config.h"
#include "drivers/backlight.h"
#include "drivers/lcd-driver.h"
#include "storage/eeprom2.h"

static float batteryVoltage = 0.0f;
static int batteryAdc = 0;

void disableDevice() {
    if (getBatteryEnabledEeprom() && isBatteryLow()) {
        char vbuf[6];
        dtostrf(batteryVoltage, 4, 1, vbuf);

        clearLCD();
        setCursorLCD(0, 0);
        printfLCD("Vbat=%sV", vbuf);
        setCursorLCD(0, 1);
        printLCD("Low Vbat!!!");
        delay(DELAY_SHUTDOWN);
        setBacklightPercent(MIN_PERCENT);
        offLCD();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_cpu();
    }
}

void initBattery() {
    if (!getBatteryEnabledEeprom()) return;
    pinMode(BATTERY_PIN, INPUT_PULLUP);
    updateBattery();
    disableDevice();
}

float getBatteryVoltage() {
    batteryAdc = analogRead(BATTERY_PIN);
    batteryVoltage = (ARDUINO_VOLTAGE_REF / 1023.0f) * batteryAdc;
    return batteryVoltage;
}

int getBatteryPercent() {
    if (batteryVoltage <= BATTERY_VOLTAGE_MIN) return 0;
    if (batteryVoltage >= BATTERY_VOLTAGE_MAX) return 100;
    return (int)((batteryVoltage - BATTERY_VOLTAGE_MIN) / (BATTERY_VOLTAGE_MAX - BATTERY_VOLTAGE_MIN) * 100.0f);
}

int getBatteryAdcValue() {
    return batteryAdc;
}

bool isBatteryLow() {
    return batteryVoltage <= BATTERY_VOLTAGE_MIN;
}

bool shouldDimBacklight() {
    return getBatteryPercent() < BATTERY_LOW_PERCENT;
}

void updateBattery() {
    if (!getBatteryEnabledEeprom()) return;

    static unsigned long lastBatteryUpdate = 0;

    if (millis() - lastBatteryUpdate >= BATTERY_UPDATE_MS) {
        batteryVoltage = getBatteryVoltage();
        lastBatteryUpdate = millis();
        disableDevice();
    }
}
