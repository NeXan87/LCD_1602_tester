#pragma once

#include "config.h"

void initScreenDiagnostics();

/// Обновление экрана (навигация по подменю и режимам)
/// @return true, если пользователь вышел (LEFT)
bool updateScreenDiagnostics();
