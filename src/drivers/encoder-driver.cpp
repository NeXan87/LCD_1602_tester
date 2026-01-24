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

static void updateEncoderButtonR(EncoderState* encoder) {
    bool isRHigh = (digitalRead(ENCODER_PIN_R) == HIGH);

    if (isRHigh) {
        encoder->rPresent = true;
        encoder->lastRHighTime = millis();
    } else {
        if (millis() - encoder->lastRHighTime > ENCODER_LAST_R_HIGH_TIME) {
            encoder->rPresent = false;
        }
    }

    uint8_t currentR = isRHigh ? 1 : 0;
    if (currentR == 1 && encoder->lastRState == 0) {
        encoder->rRisingEdge = true;
    }
    encoder->lastRState = currentR;
}

static void updateEncoderSpeed(EncoderState* encoder) {
    unsigned long now = millis();
    if (now - encoder->speedLastTime >= ENCODER_SPEED_TIME_MS) {
        int deltaPos = abs(encoder->position - encoder->speedLastPosition);
        encoder->speed = deltaPos;
        encoder->speedLastPosition = encoder->position;
        encoder->speedLastTime = now;
    }
}

static void updateIndexPulseCount(EncoderState* encoder) {
    // Запуск нового цикла по фронту R
    if (encoder->rRisingEdge) {
        encoder->rRisingEdge = false;  // сброс флага

        if (encoder->inIndexCycle) {
            // Предыдущий цикл завершён — можно сохранить результат (опционально)
            // Например: encoder->lastIndexPulseCount = encoder->indexPulseCount;
        }

        // Начало нового цикла
        encoder->inIndexCycle = true;
        encoder->indexDirection = encoder->direction;
        encoder->indexLastPosition = encoder->position;
        encoder->indexPulseCount = 0;
    }

    // Сброс при смене направления во время цикла
    if (encoder->inIndexCycle &&
        encoder->direction != 0 &&
        encoder->direction != encoder->indexDirection) {
        encoder->inIndexCycle = false;
        encoder->indexPulseCount = 0;
    }

    // Накопление импульсов
    if (encoder->inIndexCycle) {
        encoder->indexPulseCount += abs(encoder->position - encoder->indexLastPosition);
        encoder->indexLastPosition = encoder->position;
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

static void updateWaveformHistory(EncoderState* encoder, int a, int b) {
    int symbolA = getWaveformSymbol(a, encoder->prevA);
    int symbolB = getWaveformSymbol(b, encoder->prevB);

    encoder->historyA[encoder->historyIndex] = symbolA;
    encoder->historyB[encoder->historyIndex] = symbolB;
    encoder->historyIndex = (encoder->historyIndex + 1) % WAVEFORM_HISTORY_SIZE;
    encoder->prevA = a;
    encoder->prevB = b;
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

void initEncoder(EncoderState* encoder) {
    encoderState = encoder;

    pinMode(ENCODER_PIN_A, INPUT);
    pinMode(ENCODER_PIN_B, INPUT);
    pinMode(ENCODER_PIN_R, INPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoderInterrupt, CHANGE);
    resetEncoder(encoder);
}

void updateEncoder(EncoderState* encoder) {
    updateEncoderButtonR(encoder);
    updateEncoderSpeed(encoder);
    updateIndexPulseCount(encoder);

    if (millis() - encoder->lastTime > ENCODER_NO_CHANGE_TIME_MS) {
        encoder->direction = 0;
    }
}

void resetEncoder(EncoderState* encoder) {
    encoder->position = 0;
    encoder->direction = 0;
    encoder->lastTime = millis();
    encoder->speedLastTime = millis();
    encoder->speedLastPosition = 0;
    encoder->speed = 0;
    encoder->aEverHigh = false;
    encoder->bEverHigh = false;
    encoder->rPresent = false;
    encoder->errors = 0;
    encoder->lastState = 0;
    encoder->indexPulseCount = 0;
    encoder->inIndexCycle = false;
    encoder->indexDirection = 0;
    encoder->rRisingEdge = false;
    encoder->lastRState = 0;
    encoder->indexLastPosition = 0;
    encoder->lastRHighTime = 0;
    encoder->historyIndex = 0;
    encoder->prevA = 0;
    encoder->prevB = 0;

    // --- Инициализация осциллограммы ---
    int a = digitalRead(ENCODER_PIN_A);
    int b = digitalRead(ENCODER_PIN_B);
    int symbolA = getWaveformSymbol(a, a);
    int symbolB = getWaveformSymbol(b, b);

    for (uint8_t i = 0; i < WAVEFORM_HISTORY_SIZE; i++) {
        encoder->historyA[i] = symbolA;
        encoder->historyB[i] = symbolB;
    }
}
