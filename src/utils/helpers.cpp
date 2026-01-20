#include "helpers.h"

int percentToPwm(int percent) {
    return (percent * 255) / 100;
}

int clamp(int value, int min, int max) {
    if (value > max) value = max;
    if (value < min) value = min;
    return value;
}
