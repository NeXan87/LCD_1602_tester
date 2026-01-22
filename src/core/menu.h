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
    ScreenId exitScreen;
    bool isInitialized;
    SubMenuInitFunc initFunc;
    MenuScreenMapper mapper;
} ScreenMenu;

// Основной обновлятор
ScreenId updateScreenMenu(ScreenMenu* menu);

// Вспомогательные функции
void drawMenu(ScreenMenu* menu, uint8_t lcdRows);
bool isScreenMenuMoveUp(ScreenMenu* menu, uint8_t lcdRows);
bool isScreenMenuMoveDown(ScreenMenu* menu, uint8_t lcdRows);
