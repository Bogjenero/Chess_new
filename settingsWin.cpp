#include "settingsWin.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using namespace sf;


settingsWin::settingsWin() : buttonTextBack(font, load_string(BACK), 30), 
buttonBack(Vector2f(200, 60)), buttonTextReset(font, load_string(RESET), 30), buttonReset(Vector2f(200, 60)),
backgroundSprite(backgroundTexture), selectBox(Vector2f(200, 60)), selectBoxText(font, L"Select Board", 30), 
selectedText(font, L"Selected: ", 30),applyChangesButton(Vector2f(200, 60)), 
buttonTextApplyChanges(font, load_string(APPLY), 30), 
checkBox(Vector2f(14.f, 14.f)),
muteText(font, load_string(MUTE), 30), isChecked(false), selectedIndex(0) ,
boardStyleText(font, load_string(BOARD_STYLE), 22)
{
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
    nlohmann::json settings;
    file >> settings;

     if (!font.openFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load texture file: " + std::string("arial.ttf"));
    }

    height = settings["window"]["height"].get<int>();
    width = settings["window"]["width"].get<int>();
        
    int selectedIndex = 0;

    muteText.setPosition(Vector2f((width - 200) / 2.f, 130));
    muteText.setFillColor(Color::White);

    checkBox.setPosition(Vector2f((width - 40) / 2.f, 143));
    checkBox.setFillColor(Color::White);
    checkBox.setOutlineThickness(2);

    isChecked = settings["UserOptions"]["mute"].get<bool>();

    check.setSize({14.f, 14.f});
    check.setPosition(Vector2f((width - 40) / 2.f, 143));
    check.setFillColor(isChecked ? Color::Green : Color::White);


    applyChangesButton.setPosition(Vector2f((width - 200) / 2.f, 400));
    applyChangesButton.setFillColor(Color::Green);
    applyChangesButton.setOutlineThickness(2);

    Vector2f buttonPos = applyChangesButton.getPosition();
    Vector2f buttonSize = applyChangesButton.getSize();
    buttonTextApplyChanges.setPosition(Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextApplyChanges.setFillColor(Color::White);

    FloatRect textBoundsApply = buttonTextApplyChanges.getLocalBounds();

    buttonTextApplyChanges.setOrigin(Vector2f(textBoundsApply.position.x + textBoundsApply.size.x / 2.0f, textBoundsApply.position.y + textBoundsApply.size.y / 2.0f));

    drawBox();      

    buttonReset.setPosition(Vector2f((width - 200) / 2.f, 500));
    buttonReset.setFillColor(Color::Green);
    buttonReset.setOutlineThickness(2);

    buttonBack.setPosition(Vector2f((width - 200) / 2.f, 600));
    buttonBack.setFillColor(Color::Blue);
    buttonBack.setOutlineThickness(2);

    FloatRect textBounds = buttonTextBack.getLocalBounds();
    buttonTextBack.setOrigin(Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));

    buttonPos = buttonBack.getPosition();
    buttonSize = buttonBack.getSize();
    buttonTextBack.setPosition(Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextBack.setFillColor(Color::White);

    FloatRect textBoundsReset = buttonTextReset.getLocalBounds();
    buttonTextReset.setOrigin(Vector2f(textBoundsReset.position.x + textBoundsReset.size.x / 2.0f, textBoundsReset.position.y + textBoundsReset.size.y / 2.0f));

    buttonPos = buttonReset.getPosition();
    buttonSize = buttonReset.getSize();
    buttonTextReset.setPosition(Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextReset.setFillColor(Color::White);

    if(!backgroundTexture.loadFromFile(settings["settings_background"].get<std::string>()))
    {
        throw std::runtime_error("Failed to load texture file: " + std::string("./images/settings_background.png"));
    }

    backgroundTexture.setSmooth(true);
    backgroundSprite.setTexture(backgroundTexture, true);

    Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = static_cast<float>(800) / textureSize.x;
    float scaleY = static_cast<float>(800) / textureSize.y;
    backgroundSprite.setScale(Vector2f(scaleX, scaleY));

    sf::Vector2u textureSizeSettings = backgroundTexture.getSize();
    float scaleXSettings = static_cast<float>(800) / textureSizeSettings.x;
    float scaleYSettings = static_cast<float>(800) / textureSizeSettings.y;
    backgroundSprite.setScale(Vector2f(scaleXSettings, scaleYSettings));
}

void settingsWin::drawBox() {
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
    
    file >> settings;
    int index  = settings["UserOptions"]["board_style_index"];
    
    boardStyleText.setPosition(Vector2f((width - 200) / 2.f, 170));
    boardStyleText.setFillColor(Color::White);
    
    for (size_t i = 0; i < boardOptions.size(); ++i) {
        RectangleShape box(Vector2f(200, 30));
        if(index == i) {
            box.setFillColor(Color(100, 100, 200)); 
        } else {
            box.setFillColor(Color(150, 150, 250));
        }
        box.setPosition(Vector2f((width - 200) / 2.f, 200 + i * 30)); 
        optionBoxes.push_back(box);

        Text text(font, load_string(boardOptions[i]), 20);
        text.setFillColor(Color::White);
        text.setPosition(Vector2f((width - 195) / 2.f, 205 + i * 30));
        optionTexts.push_back(text);
    }
}    
    