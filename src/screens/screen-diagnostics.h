#pragma once

#include "config.h"
#include "core/screen-id.h"

void initScreenDiagnostics();

/// Обновление экрана (навигация по подменю и режимам)
/// @return идентификатор следующего экрана
ScreenId updateScreenDiagnostics();
