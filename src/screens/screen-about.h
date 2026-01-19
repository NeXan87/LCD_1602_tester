#pragma once

#include "config.h"

void initAboutScreen();

/// Обновляет экран с учётом навигации
/// @return true, если пользователь вышел (например, нажал SELECT или LEFT)
bool updateAboutScreen();
