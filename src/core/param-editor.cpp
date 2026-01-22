#include "param-editor.h"

#include <string.h>  // для memcpy

#include "drivers/buttons.h"
#include "utils/hold-navigate.h"

static void applyOriginalValue(ParamEditor* editor) {
    memcpy(editor->currentValue, editor->originalValue, editor->valueSize);
}

ScreenId updateParamEditor(ParamEditor* editor) {
    if (!editor->isInitialized) {
        // Загружаем оригинальное значение из хранилища
        editor->getFromStorage(editor->originalValue);
        editor->isInitialized = true;
        editor->drawHandler(editor->currentValue, editor->title);

        if (editor->initFunc != nullptr) {
            editor->initFunc();
        }

        applyOriginalValue(editor);
    }

    bool changed = false;

    if (clickUpButton()) {
        if (editor->stepHandler(editor->currentValue, true, editor->stepContext)) {
            editor->drawHandler(editor->currentValue, editor->title);
            changed = true;
        }
    }
    if (clickDownButton()) {
        if (editor->stepHandler(editor->currentValue, false, editor->stepContext)) {
            editor->drawHandler(editor->currentValue, editor->title);
            changed = true;
        }
    }

    // Автопрокрутка
    handleHoldNavigation(
        isUpButtonPressed(),
        isDownButtonPressed(),
        [](bool isUp, void* userData) -> bool {
            ParamEditor* ed = (ParamEditor*)userData;
            bool res = ed->stepHandler(ed->currentValue, isUp, ed->stepContext);
            if (res) {
                ed->drawHandler(ed->currentValue, ed->title);
            }
            return res;
        },
        editor,
        STEP_INTERVAL_FAST_MS);

    if (clickLeftButton()) {
        // Отмена — восстанавливаем оригинальное значение
        applyOriginalValue(editor);
        editor->saveToStorage(editor->originalValue);  // или setBacklightPercent(original)
        editor->isInitialized = false;
        return editor->exitScreen;
    }

    if (clickSelectButton()) {
        // Подтверждение — сохраняем
        editor->saveToStorage(editor->currentValue);
        editor->isInitialized = false;
        return editor->exitScreen;
    }

    return SCREEN_NONE;  // должен быть определён как -1
}
