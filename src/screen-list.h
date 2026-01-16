#pragma once

#include "config.h"

/// Обновляет состояние списка (обработка кнопок, перерисовка)
/// @return ScreenId выбранного экрана, если был подтверждён SELECT; иначе SCREEN_LIST
ScreenId screenListUpdate();

/// Принудительно обновить отображение (например, при возврате из другого экрана)
void screenListRedraw();
