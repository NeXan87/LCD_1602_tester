#include "encoder-driver.h"

#include <Arduino.h>

#include "config.h"

// Пины энкодера
static uint8_t pinA, pinB, pinR;
static EncoderState* encoderState;

// Прерывание для A
void encoderAInterrupt() {
    uint8_t a = digitalRead(pinA);
    uint8_t b = digitalRead(pinB);
    uint8_t state = (a << 1) | b;

    if (a) encoderState->aEverHigh = true;
    if (b) encoderState->bEverHigh = true;

    // Определение направления и позиции
    static uint8_t prevState = 0;
    if (prevState != state) {
        // Квадратурная логика
        if ((prevState == 0b00 && state == 0b01) ||
            (prevState == 0b01 && state == 0b11) ||
            (prevState == 0b11 && state == 0b10) ||
            (prevState == 0b10 && state == 0b00)) {
            encoderState->position++;
            encoderState->direction = 1;  // CW
        } else if ((prevState == 0b00 && state == 0b10) ||
                   (prevState == 0b10 && state == 0b11) ||
                   (prevState == 0b11 && state == 0b01) ||
                   (prevState == 0b01 && state == 0b00)) {
            encoderState->position--;
            encoderState->direction = -1;  // CCW
        } else {
            // Пропуск импульса
            encoderState->errors++;
        }
        prevState = state;
        encoderState->lastTime = millis();
    }
}

// Прерывание для B (опционально, но для полноты)
void encoderBInterrupt() {
    // Аналогично, но поскольку A уже обрабатывает, можно оставить пустым или дублировать логику
    encoderAInterrupt();
}

void initEncoder(EncoderState* state) {
    encoderState = state;

    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_PIN_C_R, INPUT);

    // Прерывания на rising/falling для A и B
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderAInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderBInterrupt, CHANGE);
    resetEncoder(state);
}

void updateEncoder(EncoderState* state) {
    // Проверка R
    state->rPresent = digitalRead(pinR) == HIGH;

    // Расчёт скорости (имп/сек)
    static unsigned long lastPosTime = 0;
    static long lastPos = 0;
    unsigned long now = millis();
    if (now - lastPosTime >= ENCODER_SPEED_TIME_MS) {
        long deltaPos = abs(state->position - lastPos);
        state->speed = deltaPos;
        lastPos = state->position;
        lastPosTime = now;
    }

    // Если долго нет изменений, direction = 0
    if (millis() - state->lastTime > ENCODER_NO_CHANGE_TIME_MS) {
        state->direction = 0;
    }
}

void resetEncoder(EncoderState* state) {
    state->position = 0;
    state->direction = 0;
    state->lastTime = millis();
    state->speed = 0;
    state->aEverHigh = false;
    state->bEverHigh = false;
    state->rPresent = false;
    state->errors = 0;
    state->lastState = 0;
}
