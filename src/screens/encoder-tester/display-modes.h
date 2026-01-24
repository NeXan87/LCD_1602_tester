#pragma once

#include "drivers/encoder-driver.h"

void drawDiagnostics(const EncoderState* encoder);
void drawPositionSpeed(const EncoderState* encoder);
void drawWaveform(const EncoderState* encoder);
