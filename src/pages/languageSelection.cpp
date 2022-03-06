#include "languageSelection.h"
#include "utils.h"
#include <Preferences.h>

LanguageSelection::LanguageSelection(Minitel* m) :
    Page {m} {
    _state = STATE_NEW;
}

MenuItem LanguageSelection::run() {
    switch (_state) {
    case STATE_NEW:
        showPage();
        _minitel->echo(false);
        _state = STATE_WAITING_FOR_INPUT;
        break;
    case STATE_WAITING_FOR_INPUT: {
        Languages i = getInput();
        if (i != LANGUAGES_LAST) {
            extern Preferences preferences;
            preferences.putUShort("language_index", (uint16_t)i);
            l10n.setLanguage(i);
            _minitel->echo(true);
            return MenuItem::MenuOutput_HOME;
        }
        // else ignore
        break;
    }
    default:
        break;
    }
    return MenuItem::MenuOutput_NONE;
}

void LanguageSelection::showPage() {
    Page::showTitle(l10n.get(L10N_STRINGS::LANGUAGE_PAGE_TITLE));

    _minitel->moveCursorDown(1);

    for (uint8_t l = LANGUAGES_FIRST; l != LANGUAGES_LAST; ++l) {
        showInput(l + 1, l10n.get(LANGUAGE_NAME, (Languages)l).c_str());
    }
}

void LanguageSelection::showInput(const int num, const char* title) {
    _minitel->attributs(INVERSION_FOND);
    _minitel->print(String(num));
    _minitel->attributs(FOND_NORMAL);
    _minitel->print(" - ");
    _minitel->println(title);
}

Languages LanguageSelection::getInput() {
    const char key = ghettoAtoi(_minitel->getKeyCode()) - 1;
    if (key < LANGUAGES_LAST) {
        return (Languages)key;
    }
    return LANGUAGES_LAST;
}