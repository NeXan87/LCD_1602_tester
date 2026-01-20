#include "battery.h"

#include "config.h"
#include "storage/eeprom2.h"

static float batteryVoltage = 0.0f;
static int batteryAdc = 0;

void initBattery() {
    pinMode(BATTERY_PIN, INPUT);
    updateBattery();
}

float getBatteryVoltage() {
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
        batteryAdc = analogRead(BATTERY_PIN);
        batteryVoltage = (ARDUINO_VOLTAGE_REF / 1023.0f) * batteryAdc;
        lastBatteryUpdate = millis();

        // if (getBatteryEnabledEeprom() && isBatteryLow()) {
        //     clearLCD();
        //     setCursorLCD(0, 0);
        //     printfLCD("Vbat=%.1fV", getBatteryVoltage());
        //     setCursorLCD(0, 1);
        //     printLCD("Low Vbat!!!");
        //     delay(2000);  // Показать на 2 секунды
        //     // Остановить устройство, например, войти в бесконечный цикл или сон
        //     while (true) {
        //         // Ничего не делать
        //     }
        // }
    }
}
