#pragma once

#include <stdint.h>

#include "core/screen-id.h"

typedef void (*SubMenuInitFunc)(void);

typedef struct {
    const char* title;
    const char* const* items;
    uint8_t count;
    int selectedIndex;
    ScreenId screenId;
    bool isInitialized;
    SubMenuInitFunc initFunc;
} ScreenSubMenu;

// Callback: преобразует индекс пункта → ScreenId
typedef ScreenId (*SubMenuScreenMapper)(int index);

// Универсальный обновлятор
ScreenId updateSubMenu(ScreenSubMenu* menu, SubMenuScreenMapper mapper);
