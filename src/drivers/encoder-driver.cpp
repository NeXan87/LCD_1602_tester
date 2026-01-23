#include "encoder-driver.h"

#include <Arduino.h>

#include "config.h"

static EncoderState* encoderState;

// Таблица переходов для квадратурного энкодера
// Индекс: [prevA prevB currA currB] → шаг
static const int8_t QUADRATURE_TABLE[16] = {
    // prev=00
    0, -1, 1, 0,  // curr=00,01,10,11
    // prev=01
    1, 0, 0, -1,  // curr=00,01,10,11
    // prev=10
    -1, 0, 0, 1,  // curr=00,01,10,11
    // prev=11
    0, 1, -1, 0  // curr=00,01,10,11
};

void encoderInterrupt() {
    int a = digitalRead(ENCODER_PIN_A);
    int b = digitalRead(ENCODER_PIN_B);
    int currentState = (a << 1) | b;
    int prevState = encoderState->lastState;

    // Формируем индекс: 4 бита = [prev][curr]
    int index = (prevState << 2) | currentState;
    int8_t step = QUADRATURE_TABLE[index];

    if (step != 0) {
        encoderState->position += step;
        encoderState->direction = (step > 0) ? 1 : -1;
        encoderState->lastTime = millis();
        encoderState->lastState = currentState;

        if (a) encoderState->aEverHigh = true;
        if (b) encoderState->bEverHigh = true;
    } else {
        encoderState->errors++;
    }
}

void initEncoder(EncoderState* state) {
    encoderState = state;

    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_PIN_C_R, INPUT);

    // Прерывания на rising/falling для A и B
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderInterrupt, CHANGE);
    resetEncoder(state);
}

void updateEncoder(EncoderState* state) {
    // Проверка R
    state->rPresent = digitalRead(ENCODER_PIN_C_R) == HIGH;

    // Расчёт скорости (имп/сек)
    static unsigned long lastPosTime = 0;
    static long lastPos = 0;
    unsigned long now = millis();
    if (now - lastPosTime >= ENCODER_SPEED_TIME_MS) {
        int deltaPos = abs(state->position - lastPos);
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
