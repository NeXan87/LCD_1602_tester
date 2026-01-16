#pragma once

#include "config.h"

/// Инициализация экрана диагностики
void initScreenDiagnostics();

/// Обновление экрана (навигация по подменю и режимам)
/// @return true, если пользователь вышел (LEFT/SELECT)
bool updateScreenDiagnostics();
