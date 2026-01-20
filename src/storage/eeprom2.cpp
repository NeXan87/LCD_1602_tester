#include "storage/eeprom2.h"

#include <EEPROM.h>

#include "config.h"
#include "utils/helpers.h"

static Settings settings = {
    .magic = 0,
    .backlightPercent = MAX_PERCENT,
};

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
void initEeprom() {
    readSettingsFromEeprom();
    if (settings.magic != EEPROM_MAGIC_VALUE) {
        // Первый запуск или повреждённые данные
        settings.magic = EEPROM_MAGIC_VALUE;
        settings.backlightPercent = MAX_PERCENT;
        saveSettingsEeprom();
    }
}

void saveSettingsEeprom() {
    writeSettingsToEeprom();
}

int getBacklightPercentEeprom() {
    return settings.backlightPercent;
}

void setBacklightPercentEeprom(int percent) {
    percent = clamp(percent, MIN_PERCENT, MAX_PERCENT);
    settings.backlightPercent = percent;
    saveSettingsEeprom();
}
