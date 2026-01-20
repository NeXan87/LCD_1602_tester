#pragma once

#include "config.h"
#include "core/screen-id.h"

void initAboutScreen();

/// Обновляет экран с учётом навигации
/// @return ScreenId выбранного экрана
ScreenId updateAboutScreen();
