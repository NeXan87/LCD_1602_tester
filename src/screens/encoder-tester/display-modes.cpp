#include "drivers/encoder-driver.h"
#include "drivers/lcd-driver.h"

static void drawWaveformLine(const char* prefix, const int* history, int historyIndex) {
    printLCD(prefix);
    for (uint8_t i = 0; i < WAVEFORM_HISTORY_SIZE; i++) {
        int idx = (historyIndex + i) % WAVEFORM_HISTORY_SIZE;
        writeCharLCD((uint8_t)history[idx]);
    }
}

void drawDiagnostics(const EncoderState* encoder) {
    const char* aStatus = encoder->aEverHigh ? "OK" : "NO";
    const char* bStatus = encoder->bEverHigh ? "OK" : "NO";
    const char* rStatus = encoder->rPresent ? "\x04" : " ";

    setCursorLCD(0, 0);
    printfLCD("A:%s B:%s R:%s", aStatus, bStatus, rStatus);

    setCursorLCD(0, 1);
    printfLCD("ERR:%-12lu", encoder->errors);
}

void drawPositionSpeed(const EncoderState* encoder) {
    char dirChar = '-';
    if (encoder->direction == 1) {
        dirChar = 126;  // right arrow
    } else if (encoder->direction == -1) {
        dirChar = 127;  // left arrow
    }

    setCursorLCD(0, 0);
    printfLCD("P:%-6ld D:%c", encoder->position, dirChar);

    setCursorLCD(0, 1);
    printfLCD("I:%-4ld   S:%-4d", encoder->indexPulseCount, encoder->speed);
}

void drawWaveform(const EncoderState* encoder) {
    setCursorLCD(0, 0);
    drawWaveformLine("A:", encoder->historyA, encoder->historyIndex);

    setCursorLCD(0, 1);
    drawWaveformLine("B:", encoder->historyB, encoder->historyIndex);
}
