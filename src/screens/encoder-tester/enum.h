#pragma once

typedef enum {
    ENCODER_DISPLAY_DIAGNOSTICS,
    ENCODER_DISPLAY_POSITION_SPEED,
    ENCODER_DISPLAY_WAVEFORM,
    ENCODER_DISPLAY_MODE_COUNT
} EncoderDisplayMode;

typedef enum {
    ENCODER_TYPE_ROTARY,  // Угловой (поворотный)
    ENCODER_TYPE_LINEAR   // Линейный
} EncoderType;

typedef enum {
    TTL,
    TTL_DIFF,
} EncoderInterfaces;
