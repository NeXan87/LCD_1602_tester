#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "core/screen-id.h"

// Callback'и для работы со значением
typedef void (*ParamGetFunc)(void* valueOut);
typedef void (*ParamSetFunc)(const void* value);
typedef bool (*ParamStepFunc)(void* value, bool isUp, const void* context);
typedef void (*ParamDrawFunc)(const void* value, const char* title);
typedef void (*EditorInitFunc)(void);
typedef void (*ParamApplyFunc)(const void* value);

// Контекст редактора
typedef struct {
    const char* title;
    ScreenId screenId;
    ScreenId exitScreen;
    void* currentValue;
    void* originalValue;
    uint8_t valueSize;
    ParamGetFunc getFromStorage;  // загрузка из EEPROM
    ParamSetFunc saveToStorage;   // сохранение в EEPROM
    ParamStepFunc stepHandler;    // изменение значения (up/down)
    ParamDrawFunc drawHandler;    // отрисовка
    ParamApplyFunc applyFunc;     // сохранение значения
    EditorInitFunc initFunc;

    // Доп. контекст для stepHandler (например, min/max)
    const void* stepContext;
    bool isInitialized;
} ParamEditor;

// Основной обновлятор
ScreenId updateParamEditor(ParamEditor* editor);
