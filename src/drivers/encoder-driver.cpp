#include "encoder-driver.h"

#include <Arduino.h>

#include "config.h"
#include "drivers/lcd-custom-chars.h"

static EncoderState* encoderState;
static EncoderInterfaces encoderInteface;

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

    uint8_t currentR = isRHigh ? 1 : 0;
    if (currentR == 1 && state->lastRState == 0) {
        state->rRisingEdge = true;
    }
    state->lastRState = currentR;
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

static void updateIndexPulseCount(EncoderState* state) {
    // Запуск нового цикла по фронту R
    if (state->rRisingEdge) {
        state->rRisingEdge = false;  // сброс флага

        if (state->inIndexCycle) {
            // Предыдущий цикл завершён — можно сохранить результат (опционально)
            // Например: encoder->lastIndexPulseCount = encoder->indexPulseCount;
        }

        // Начало нового цикла
        state->inIndexCycle = true;
        state->indexDirection = state->direction;
        state->indexLastPosition = state->position;
        state->indexPulseCount = 0;
    }

    // Сброс при смене направления во время цикла
    if (state->inIndexCycle &&
        state->direction != 0 &&
        state->direction != state->indexDirection) {
        state->inIndexCycle = false;
        state->indexPulseCount = 0;
    }

    // Накопление импульсов
    if (state->inIndexCycle) {
        state->indexPulseCount += abs(state->position - state->indexLastPosition);
        state->indexLastPosition = state->position;
    }
    // Вне цикла — счётчик остаётся последним (или 0), но не сбрасывается автоматически
    // (если нужно сбрасывать — добавьте else { encoder->indexPulseCount = 0; })
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

    if (encoderInteface == TTL_DIFF) {
        encoderState->aPlusRaw = analogRead(ENC_A_PLUS_PIN);
        encoderState->aMinusRaw = analogRead(ENC_A_MINUS_PIN);
        encoderState->bPlusRaw = analogRead(ENC_B_PLUS_PIN);
        encoderState->bMinusRaw = analogRead(ENC_B_MINUS_PIN);
        encoderState->rPlusRaw = analogRead(ENC_R_PLUS_PIN);
        encoderState->rMinusRaw = analogRead(ENC_R_MINUS_PIN);

        int aDiff = encoderState->aPlusRaw - encoderState->aMinusRaw;
        int bDiff = encoderState->bPlusRaw - encoderState->bMinusRaw;
        int rDiff = encoderState->rPlusRaw - encoderState->rMinusRaw;

        if (aDiff > ENCODER_DIFF_THRESHOLD) encoderState->aDiffEverHigh = true;
        if (aDiff < -ENCODER_DIFF_THRESHOLD) encoderState->aDiffEverLow = true;
        if (bDiff > ENCODER_DIFF_THRESHOLD) encoderState->bDiffEverHigh = true;
        if (bDiff < -ENCODER_DIFF_THRESHOLD) encoderState->bDiffEverLow = true;
        if (rDiff > ENCODER_DIFF_THRESHOLD) encoderState->rDiffEverHigh = true;
        if (rDiff < -ENCODER_DIFF_THRESHOLD) encoderState->rDiffEverLow = true;
    }
}

void initEncoder(EncoderState* state, EncoderInterfaces interface) {
    encoderState = state;
    encoderInteface = interface;

    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_PIN_R, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderInterrupt, CHANGE);
    resetEncoder(state);
}

void detachEncoder() {
    detachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A));
    detachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B));
    encoderState = nullptr;
}

void updateEncoder(EncoderState* state) {
    updateEncoderButtonR(state);
    updateEncoderSpeed(state);
    updateIndexPulseCount(state);

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
    state->indexPulseCount = 0;
    state->inIndexCycle = false;
    state->indexDirection = 0;
    state->rRisingEdge = false;
    state->lastRState = 0;
    state->indexLastPosition = 0;
    state->lastRHighTime = 0;
    state->historyIndex = 0;
    state->prevA = 0;
    state->prevB = 0;
    state->aDiffEverHigh = false;
    state->aDiffEverLow = false;
    state->bDiffEverHigh = false;
    state->bDiffEverLow = false;
    state->rDiffEverHigh = false;
    state->rDiffEverLow = false;

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
