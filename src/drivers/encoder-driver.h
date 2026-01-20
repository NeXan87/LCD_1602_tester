#pragma once

#include <Arduino.h>

// Структура состояния энкодера
struct EncoderState {
    volatile long position;       // Текущая позиция
    volatile int direction;       // Направление: 1 - CW, -1 - CCW, 0 - стоп
    volatile unsigned long lastTime; // Время последнего изменения
    volatile int speed;           // Скорость в имп/сек
    volatile bool aEverHigh;      // Был ли сигнал A высоким
    volatile bool bEverHigh;      // Был ли сигнал B высоким
    volatile bool rPresent;       // Наличие сигнала R (индекс)
    volatile unsigned long errors; // Количество ошибок (пропуски импульсов)
    volatile uint8_t lastState;   // Последнее состояние A/B
};

// Инициализация драйвера энкодера
// Пины: A - pinA, B - pinB, R - pinR
void initEncoder(uint8_t pinA, uint8_t pinB, uint8_t pinR, EncoderState* state);

// Обновление состояния энкодера (вызывать в loop)
void updateEncoder(EncoderState* state);

// Сброс состояния
void resetEncoder(EncoderState* state);