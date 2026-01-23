#include "param-editor.h"

#include <string.h>

#include "drivers/buttons.h"
#include "utils/hold-navigate.h"

static void applyOriginalValue(ParamEditor* editor) {
    memcpy(editor->currentValue, editor->originalValue, editor->valueSize);
}

static bool paramEditorStepCallback(bool isUp, void* userData) {
    ParamEditor* editor = (ParamEditor*)userData;
    bool isChanged = editor->stepHandler(editor->currentValue, isUp, editor->stepContext);
    if (isChanged) {
        editor->drawHandler(editor->currentValue, editor->title);
    }
    return isChanged;
}

ScreenId updateParamEditor(ParamEditor* editor) {
    if (!editor->isInitialized) {
        editor->getFromStorage(editor->originalValue);
        applyOriginalValue(editor);

        if (editor->initFunc != nullptr) {
            editor->initFunc();
        }

        editor->drawHandler(editor->currentValue, editor->title);
        editor->isInitialized = true;
    }

    bool isChanged = false;

    if (clickUpButton()) {
        isChanged = editor->stepHandler(editor->currentValue, true, editor->stepContext);

        if (isChanged) {
            editor->drawHandler(editor->currentValue, editor->title);
        }
    }

    if (clickDownButton()) {
        isChanged = editor->stepHandler(editor->currentValue, false, editor->stepContext);

        if (isChanged) {
            editor->drawHandler(editor->currentValue, editor->title);
        }
    }

    handleHoldNavigation(paramEditorStepCallback, editor, STEP_INTERVAL_FAST_MS);

    if (clickLeftButton()) {
        applyOriginalValue(editor);

        if (editor->applyFunc != nullptr) {
            editor->applyFunc(editor->originalValue);
        }

        editor->saveToStorage(editor->originalValue);
        editor->isInitialized = false;
        return editor->exitScreen;
    }

    if (clickSelectButton()) {
        editor->saveToStorage(editor->currentValue);
        editor->isInitialized = false;
        return editor->exitScreen;
    }

    return editor->screenId;
}
