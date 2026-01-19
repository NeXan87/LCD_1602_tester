#include "helpers.h"

int percentToPwm(int percent) {
    return (percent * 255) / 100;
}
