#pragma once

#include "config.h"
#include "core/screen-id.h"

/// Обновляет состояние списка (обработка кнопок, перерисовка)
/// @return ScreenId выбранного экрана, если был подтверждён; иначе SCREEN_LIST
ScreenId updateScreenList();

/// Принудительно обновить отображение (например, при возврате из другого экрана)
void redrawScreenList();
