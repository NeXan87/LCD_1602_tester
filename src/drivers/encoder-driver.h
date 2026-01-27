#pragma once

#include <Arduino.h>

#include "screens/encoder-tester/enum.h"

#define WAVEFORM_HISTORY_SIZE 14

// Структура состояния энкодера
struct EncoderState {
    // === Основные параметры позиции и движения ===
    volatile long position;           // Текущая позиция в импульсах
    volatile int direction;           // Направление: 1 — CW, -1 — CCW, 0 — стоп
    volatile unsigned long lastTime;  // Время последнего изменения позиции

    // === Скорость ===
    volatile unsigned long speedLastTime;  // Время последнего измерения скорости
    volatile long speedLastPosition;       // Позиция при последнем измерении
    volatile int speed;                    // Текущая скорость (имп/с)

    // === Индексный импульс (R-метка) ===
    volatile bool rPresent;                // Активен ли R-сигнал сейчас
    volatile unsigned long lastRHighTime;  // Время последнего HIGH на R
    volatile long indexPulseCount;         // Импульсы между R-метками
    volatile bool inIndexCycle;            // Флаг: идёт отсчёт от R
    volatile int indexDirection;           // Направление в начале цикла
    volatile bool rRisingEdge;             // Обнаружен фронт R
    volatile uint8_t lastRState;           // Предыдущее состояние R (0/1)
    volatile long indexLastPosition;       // Позиция при активации R
    volatile bool indexPulseCountValid;    // true = подсчёт завершён, значение готово

    // === Диагностика TTL-сигналов ===
    volatile bool aEverHigh;        // Был ли A = HIGH
    volatile bool bEverHigh;        // Был ли B = HIGH
    volatile int lastState;         // Последнее состояние (A<<1 | B)
    volatile unsigned long errors;  // Счётчик ошибок (недопустимые переходы)

    // === История для осциллограммы ===
    int historyA[WAVEFORM_HISTORY_SIZE];  // История значений канала A
    int historyB[WAVEFORM_HISTORY_SIZE];  // История значений канала B
    int historyIndex;                     // Текущий индекс в буфере истории

    // === Состояние входов (для ISR) ===
    volatile int prevA;  // Предыдущее значение A
    volatile int prevB;  // Предыдущее значение B

    // === ДИФФЕРЕНЦИАЛЬНЫЕ СИГНАЛЫ (RS422/TTL с инверсией) ===
    // Прямые и инвертированные линии (сырые значения АЦП)
    volatile int16_t aPlusRaw;
    volatile int16_t aMinusRaw;
    volatile int16_t bPlusRaw;
    volatile int16_t bMinusRaw;
    volatile int16_t rPlusRaw;
    volatile int16_t rMinusRaw;

    // Диагностические флаги для дифференциальных пар
    volatile bool aDiffEverHigh;  // Была ли разность A+ – A– > порога
    volatile bool aDiffEverLow;   // Была ли разность A+ – A– < –порога
    volatile bool bDiffEverHigh;  // Была ли разность B+ – B– > порога
    volatile bool bDiffEverLow;   // Была ли разность B+ – B– < –порога
    volatile bool rDiffEverHigh;  // ← был ли R в состоянии "1"
    volatile bool rDiffEverLow;   // ← был ли R в состоянии "0"
};

// Инициализация драйвера энкодера
void initEncoder(EncoderState* state, EncoderInterfaces interface);

// Отключение энкодера
void detachEncoder();

// Обновление состояния энкодера (вызывать в loop)
void updateEncoder(EncoderState* state);

// Сброс состояния
void resetEncoder(EncoderState* state);
