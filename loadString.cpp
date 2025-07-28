#include "loadString.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <map>

const std::map <Strings, std::wstring> stringMap = {

    { START, L"Start Game" },
    { VICTORY, L"Victory!" },
    { WHITE_WINS, L"White Wins!" },
    { BLACK_WINS, L"Black Wins!" },
    { DRAW, L"Draw!" },
    { OK, L"OK" },
    { CHESS, L"Chess Game" },
    { FINISH, L"Finish the game" },
    { ENDWINDOW, L"End window" },
    { QUIT, L"Quit Game"},
    { SETTINGS, L"Settings" },
    { BACK, L"Back to Menu" },
    { RESET, L"Reset Game" },
    { ENGINE, L"Against computer" },
    { COLOR_SELECTION, L"Color Selection" },
    { CHOOSE_WHITE, L"Play as White" },
    { CHOOSE_BLACK, L"Play as Black" },
    { BLACK_WHITE, L"blackWhite" },
    { LIGHT_WOOD, L"lightWood" },
    { DARK_WOOD, L"darkWood" },
    { BLUE_GRAY, L"blueGray" },
    { GREEN_MARBLE, L"greenMarble" },
    { SLATE_DARK, L"slateDark" },
    { APPLY, L"applyChanges" },
    { MUTE, L"Mute" },
    

};

std::wstring load_string(Strings uID) {

    static const std::wstring emptyString = L"";
    auto it = stringMap.find(uID);
    return (it != stringMap.end()) ? it->second : emptyString;

}