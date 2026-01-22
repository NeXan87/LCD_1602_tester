#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "core/screen-id.h"

typedef void (*SubMenuInitFunc)(void);

// Callback: преобразует индекс → ScreenId
typedef ScreenId (*MenuScreenMapper)(int index);

typedef struct {
    const char* const* items;
    uint8_t itemCount;
    int selectedIndex;
    int topIndex;
    ScreenId screenId;
    bool isInitialized;
    SubMenuInitFunc initFunc;
} ScreenMenu;

// Основной обновлятор
ScreenId updateScreenMenu(
    ScreenMenu* menu,
    MenuScreenMapper mapper,
    ScreenId resetScreen  // экран для сброса при входе в подменю
);

// Вспомогательные функции
void drawScreenMenu(ScreenMenu* menu, uint8_t lcdRows);
bool screenMenuMoveUp(ScreenMenu* menu, uint8_t lcdRows);
bool screenMenuMoveDown(ScreenMenu* menu, uint8_t lcdRows);
