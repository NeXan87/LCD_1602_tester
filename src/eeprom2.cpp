#include "eeprom2.h"

#include <EEPROM.h>

// === СТРУКТУРА НАСТРОЕК ===
struct Settings {
    uint32_t magic;            // сигнатура для проверки валидности
    uint8_t backlightPercent;  // 0–100
    // Добавьте другие поля по мере необходимости
};

// === КОНСТАНТЫ ===
static const uint32_t SETTINGS_MAGIC = 0x54534554;  // "TEST" in ASCII
static const int EEPROM_ADDR = 0;                   // начальный адрес

// === ВНУТРЕННЕЕ СОСТОЯНИЕ ===
static Settings g_settings = {
    .magic = 0,
    .backlightPercent = 100  // значение по умолчанию
};

// === ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===
static void writeSettingsToEeprom() {
    const byte* p = (const byte*)&g_settings;
    for (int i = 0; i < sizeof(g_settings); ++i) {
        EEPROM.write(EEPROM_ADDR + i, p[i]);
    }
}

static void readSettingsFromEeprom() {
    byte* p = (byte*)&g_settings;
    for (int i = 0; i < sizeof(g_settings); ++i) {
        p[i] = EEPROM.read(EEPROM_ADDR + i);
    }
}

// === ПУБЛИЧНЫЙ ИНТЕРФЕЙС ===
void eepromInit() {
    readSettingsFromEeprom();
    if (g_settings.magic != SETTINGS_MAGIC) {
        // Первый запуск или повреждённые данные
        g_settings.magic = SETTINGS_MAGIC;
        g_settings.backlightPercent = 100;  // значение по умолчанию
        eepromSaveSettings();               // сохранить корректные значения
    }
}

void eepromSaveSettings() {
    writeSettingsToEeprom();
}

void eepromLoadSettings() {
    readSettingsFromEeprom();
    if (g_settings.magic != SETTINGS_MAGIC) {
        eepromInit();  // восстановить значения по умолчанию
    }
}

uint8_t eepromGetBacklightPercent() {
    return g_settings.backlightPercent;
}

void eepromSetBacklightPercent(uint8_t percent) {
    if (percent > 100) percent = 100;
    g_settings.backlightPercent = percent;
}
