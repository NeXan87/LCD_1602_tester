#include "encoder-driver.h"

#include <Arduino.h>

#include "config.h"
#include "drivers/lcd-custom-chars.h"

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

static void updateEncoderButtonR(EncoderState* state) {
    bool isRHigh = (digitalRead(ENCODER_PIN_R) == HIGH);

    if (isRHigh) {
        state->rPresent = true;
        state->lastRHighTime = millis();
    } else {
        if (millis() - state->lastRHighTime > ENCODER_LAST_R_HIGH_TIME) {
            state->rPresent = false;
        }
    }
}

static void updateEncoderSpeed(EncoderState* state) {
    unsigned long now = millis();
    if (now - state->speedLastTime >= ENCODER_SPEED_TIME_MS) {
        int deltaPos = abs(state->position - state->speedLastPosition);
        state->speed = deltaPos;
        state->speedLastPosition = state->position;
        state->speedLastTime = now;
    }
}

static int getWaveformSymbol(int current, int prev) {
    if (current == 0) {
        return (prev == 0) ? LCD_ENCODER_STATE_00 : LCD_ENCODER_STATE_10;
    } else {
        return (prev == 0) ? LCD_ENCODER_STATE_01 : LCD_ENCODER_STATE_11;
    }
}

static void updateWaveformHistory(EncoderState* state, int a, int b) {
    int symbolA = getWaveformSymbol(a, state->prevA);
    int symbolB = getWaveformSymbol(b, state->prevB);

    state->historyA[state->historyIndex] = symbolA;
    state->historyB[state->historyIndex] = symbolB;
    state->historyIndex = (state->historyIndex + 1) % WAVEFORM_HISTORY_SIZE;
    state->prevA = a;
    state->prevB = b;
}

void encoderInterrupt() {
    int a = digitalRead(ENCODER_PIN_A);
    int b = digitalRead(ENCODER_PIN_B);
    int currentState = (a << 1) | b;
    int prevState = encoderState->lastState;
    int index = (prevState << 2) | currentState;
    int8_t step = QUADRATURE_TABLE[index];

    updateWaveformHistory(encoderState, a, b);

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
    pinMode(ENCODER_PIN_R, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderInterrupt, CHANGE);
    resetEncoder(state);
}

void updateEncoder(EncoderState* state) {
    updateEncoderButtonR(state);
    updateEncoderSpeed(state);

    if (millis() - state->lastTime > ENCODER_NO_CHANGE_TIME_MS) {
        state->direction = 0;
    }
}

void resetEncoder(EncoderState* state) {
    state->position = 0;
    state->direction = 0;
    state->lastTime = millis();
    state->speedLastTime = millis();
    state->speedLastPosition = 0;
    state->speed = 0;
    state->aEverHigh = false;
    state->bEverHigh = false;
    state->rPresent = false;
    state->errors = 0;
    state->lastState = 0;
    state->lastRHighTime = 0;
    state->historyIndex = 0;
    state->prevA = 0;
    state->prevB = 0;

    // --- Инициализация осциллограммы ---
    int a = digitalRead(ENCODER_PIN_A);
    int b = digitalRead(ENCODER_PIN_B);
    int symbolA = getWaveformSymbol(a, a);
    int symbolB = getWaveformSymbol(b, b);

    for (uint8_t i = 0; i < WAVEFORM_HISTORY_SIZE; i++) {
        state->historyA[i] = symbolA;
        state->historyB[i] = symbolB;
    }
}
