#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "core/screen-id.h"

typedef void (*TextViewerInitFunc)(void);

typedef struct {
    const char* const* lines;
    uint8_t lineCount;
    int16_t scrollOffset;
    ScreenId screenId;
    bool isInitialized;
    TextViewerInitFunc initFunc;
} TextViewer;

/// @brief Универсальный обновлятор текстового просмотрщика
/// @param viewer Указатель на структуру TextViewer с данными (строки, смещение и т.д.)
/// @param exitScreen ScreenId экрана, на который перейти по нажатию LEFT
/// @return ScreenId следующего экрана (обычно viewer->screenId или exitScreen)
ScreenId updateTextViewer(TextViewer* viewer, ScreenId exitScreen);

/// @brief Отображает содержимое текстового просмотрщика на LCD
/// @param viewer Указатель на структуру TextViewer с данными (массив строк, текущее смещение и т.д.)
/// @param lcdRows Количество строк дисплея (обычно 2 для 1602)
/// @note Функция автоматически обрезает прокрутку по границам и отображает только видимые строки.
///       Также вызывает drawScrollIndicators() для отображения индикаторов прокрутки.
void drawTextViewer(TextViewer* viewer, uint8_t lcdRows);

/// @brief Прокручивает текст вверх на одну строку
/// @param viewer Указатель на структуру TextViewer
/// @param lcdRows Количество строк дисплея
/// @return true, если прокрутка выполнена успешно (есть куда прокручивать), иначе false
/// @note Изменяет поле scrollOffset в структуре viewer
bool textViewMoveUp(TextViewer* viewer, uint8_t lcdRows);

/// @brief Прокручивает текст вниз на одну строку
/// @param viewer Указатель на структуру TextViewer
/// @param lcdRows Количество строк дисплея
/// @return true, если прокрутка выполнена успешно (есть куда прокручивать), иначе false
/// @note Изменяет поле scrollOffset в структуре viewer
bool textViewMoveDown(TextViewer* viewer, uint8_t lcdRows);
