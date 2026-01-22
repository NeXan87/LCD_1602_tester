#pragma once

#include <stdint.h>

#include "core/screen-id.h"

typedef void (*SubMenuInitFunc)(void);

// Callback: преобразует индекс пункта → ScreenId
typedef ScreenId (*SubMenuScreenMapper)(int index);

typedef struct {
    const char* title;
    const char* const* items;
    uint8_t count;
    int selectedIndex;
    ScreenId screenId;
    ScreenId exitScreen;
    bool isInitialized;
    SubMenuInitFunc initFunc;
    SubMenuScreenMapper mapper;
} ScreenSubMenu;

// Универсальный обновлятор
ScreenId updateSubMenu(ScreenSubMenu* menu);
