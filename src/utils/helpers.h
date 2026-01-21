#pragma once

// Макрос для подсчёта элементов массива
#define COUNT_OF(x) (sizeof(x) / sizeof((x)[0]))

int percentToPwm(int percent);
int clamp(int value, int min, int max);
