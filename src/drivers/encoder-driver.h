#pragma once

#include <Arduino.h>

#define WAVEFORM_HISTORY_SIZE 14

// Структура состояния энкодера
struct EncoderState {
    volatile unsigned long lastRHighTime;  // Сбрасываем rPresent, если прошло >t мс с последнего HIGH
    volatile long position;                // Текущая позиция
    volatile int direction;                // Направление: 1 - CW, -1 - CCW, 0 - стоп
    volatile unsigned long lastTime;       // Время последнего изменения
    volatile unsigned long speedLastTime;  // Время последнего изменения
    volatile long int speedLastPosition;   // Скорость в имп/сек
    volatile int speed;                    // Скорость в имп/сек
    volatile bool aEverHigh;               // Был ли сигнал A высоким
    volatile bool bEverHigh;               // Был ли сигнал B высоким
    volatile bool rPresent;                // Наличие сигнала R (индекс)
    volatile unsigned long errors;         // Количество ошибок (пропуски импульсов)
    volatile int lastState;                // Последнее состояние A/B
    volatile long indexPulseCount;         // Счётчик импульсов между R-метками
    volatile bool inIndexCycle;            // Флаг: идёт подсчёт (R был активирован)
    volatile int indexDirection;           // Направление в начале цикла
    volatile bool rRisingEdge;             // Флаг: обнаружен фронт R
    volatile uint8_t lastRState;           // Предыдущее состояние R (0 или 1)
    volatile long indexLastPosition;       // Позиция в начале цикла
    int historyA[WAVEFORM_HISTORY_SIZE];   // текущие уровни A
    int historyB[WAVEFORM_HISTORY_SIZE];   // текущие уровни B
    int prevA;                             // предыдущее значение A
    int prevB;                             // предыдущее значение B
    int historyIndex;
};

// Инициализация драйвера энкодера
void initEncoder(EncoderState* state);

// Отключение энкодера
void detachEncoder();

// Обновление состояния энкодера (вызывать в loop)
void updateEncoder(EncoderState* state);

// Сброс состояния
void resetEncoder(EncoderState* state);
