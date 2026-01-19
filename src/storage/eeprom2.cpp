#include "storage/eeprom2.h"

#include <EEPROM.h>

#include "config.h"

static Settings settings = {
    .magic = 0,
    .backlightPercent = 100,
};

static const uint8_t EEPROM_ADDR_MAGIC = 0;             // Адрес сигнатуры валидности
static const uint32_t EEPROM_MAGIC_VALUE = 0x54534554;  // 'TEST' в ASCII

// === ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===
static void writeSettingsToEeprom() {
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&settings);
    const size_t len = sizeof(settings);
    for (size_t i = 0; i < len; ++i) {
        EEPROM.update(static_cast<int>(EEPROM_ADDR_MAGIC + i), p[i]);
    }
}

static void readSettingsFromEeprom() {
    uint8_t* p = reinterpret_cast<uint8_t*>(&settings);
    const size_t len = sizeof(settings);
    for (size_t i = 0; i < len; ++i) {
        p[i] = EEPROM.read(static_cast<int>(EEPROM_ADDR_MAGIC + i));
    }
}

// === ПУБЛИЧНЫЙ ИНТЕРФЕЙС ===
void eepromInit() {
    readSettingsFromEeprom();
    if (settings.magic != EEPROM_MAGIC_VALUE) {
        // Первый запуск или повреждённые данные
        settings.magic = EEPROM_MAGIC_VALUE;
        settings.backlightPercent = 100;
        eepromSaveSettings();
    }
}

void eepromSaveSettings() {
    writeSettingsToEeprom();
}

int eepromGetBacklightPercent() {
    return settings.backlightPercent;
}

void eepromSetBacklightPercent(int percent) {
    if (percent > 100) percent = 100;
    settings.backlightPercent = percent;
}
