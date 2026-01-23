#include "config.h"

bool toggleIsEnableIdStep(void* value, bool isUp, const void* ctx) {
    (void)isUp;
    (void)ctx;
    IsEnableId* val = (IsEnableId*)value;
    *val = (*val == ON) ? OFF : ON;
    return true;
}
