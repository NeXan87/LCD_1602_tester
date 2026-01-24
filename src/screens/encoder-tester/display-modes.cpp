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
    // Строка 1: ERR:12345
    printfLCD("ERR:%-12lu", encoder->errors);
}

void drawPositionSpeed(const EncoderState* encoder) {
    // Строка 0: Pos:12345
    setCursorLCD(0, 0);
    printfLCD("Pos:%-12ld", encoder->position);

    setCursorLCD(0, 1);
    // Строка 1: Dir:▶ S:100 i/s
    char dirChar = '-';
    if (encoder->direction == 1) {
        dirChar = 126;  // right arrow
    } else if (encoder->direction == -1) {
        dirChar = 127;  // left arrow
    }
    printfLCD("Dir:%c S:%di/s     ", dirChar, encoder->speed);
}

void drawWaveform(const EncoderState* encoder) {
    setCursorLCD(0, 0);
    drawWaveformLine("A:", encoder->historyA, encoder->historyIndex);

    setCursorLCD(0, 1);
    drawWaveformLine("B:", encoder->historyB, encoder->historyIndex);
}
