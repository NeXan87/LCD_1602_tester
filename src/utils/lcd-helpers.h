#pragma once

#include <Arduino.h>

#include "config.h"

// Ширина числового поля в редакторе
typedef enum {
    NUM_WIDTH_1 = 1,  // 0–9
    NUM_WIDTH_2 = 2,  // 0–99
    NUM_WIDTH_3 = 3,  // 0–999 (яркость, задержки)
    NUM_WIDTH_4 = 4   // 0–9999 (таймеры, счётчики)
} NumericWidth;

/// Отображает индикаторы прокрутки (стрелки) на правом краю LCD
/// @param totalItems — общее количество пунктов/строк
/// @param visibleRows — количество видимых строк (обычно LCD_ROWS)
/// @param topIndex — индекс первой отображаемой строки
void drawScrollIndicators(uint8_t totalItems, uint8_t visibleRows, int topIndex);

/// Отрисовывает одну строку пункта меню с маркером выбора
/// @param col — начальный столбец (обычно 0)
/// @param row — номер строки на дисплее (0 или 1)
/// @param text — текст пункта меню
/// @param isSelected — true, если пункт выбран
void drawMenuItem(uint8_t col, uint8_t row, const char* text, bool isSelected);

/// Отображает двухстрочное подменю с заголовком и одним пунктом
/// @param title — заголовок (например, "Settings")
/// @param itemText — текст пункта меню
/// @param isSelected — true, если пункт выбран
/// @param row — номер строки на дисплее (0 или 1)
void drawSubMenu(const char* title, const char* itemText, bool isSelected, uint8_t row = 1);

/// Обновляет индекс меню на основе нажатий кнопок
/// @param currentIndex — текущий индекс
/// @param itemCount — количество пунктов
/// @return true, если индекс изменился
bool updateMenuIndex(int* currentIndex, uint8_t itemCount);

/// Отображает редактор числового параметра с заданной шириной числа
/// @param title — заголовок (например, "Bridge")
/// @param value — текущее значение
/// @param width — ширина числового поля (1, 2, 3, 4...)
/// @param suffix — суффикс (например, "%", "s", "")
void drawNumericEditor(const char* title, int value, NumericWidth width, const char* suffix);
