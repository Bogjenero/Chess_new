#include "settingsWin.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>



settingsWin::settingsWin() : buttonTextBack(font, load_string(BACK), 30), 
buttonBack(sf::Vector2f(200, 60)), buttonTextReset(font, load_string(RESET), 30), buttonReset(sf::Vector2f(200, 60)),
backgroundSprite(backgroundTexture), selectBox(sf::Vector2f(200, 60)), selectBoxText(font, L"Select Board", 30), selectedText(font, L"Selected: ", 30),
applyChangesButton(sf::Vector2f(200, 60)), buttonTextApplyChanges(font, load_string(APPLY), 30)
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

    applyChangesButton.setPosition(sf::Vector2f((width - 200) / 2.f, 300));
    applyChangesButton.setFillColor(sf::Color::Green);
    applyChangesButton.setOutlineThickness(2);

    sf::Vector2f buttonPos = applyChangesButton.getPosition();
    sf::Vector2f buttonSize = applyChangesButton.getSize();
    buttonTextApplyChanges.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextApplyChanges.setFillColor(sf::Color::White);
    

    sf::FloatRect textBoundsApply = buttonTextApplyChanges.getLocalBounds();

    buttonTextApplyChanges.setOrigin(sf::Vector2f(textBoundsApply.position.x + textBoundsApply.size.x / 2.0f, textBoundsApply.position.y + textBoundsApply.size.y / 2.0f));
    
    drawBox();    


    buttonReset.setPosition(sf::Vector2f((width - 200) / 2.f, 400));
    buttonReset.setFillColor(sf::Color::Green);
    buttonReset.setOutlineThickness(2);

    
    buttonBack.setPosition(sf::Vector2f((width - 200) / 2.f, 500));
    buttonBack.setFillColor(sf::Color::Blue);
    buttonBack.setOutlineThickness(2);

    
    
    sf::FloatRect textBounds = buttonTextBack.getLocalBounds();
    buttonTextBack.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    
    buttonPos = buttonBack.getPosition();
    buttonSize = buttonBack.getSize();
    buttonTextBack.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextBack.setFillColor(sf::Color::White);

    sf::FloatRect textBoundsReset = buttonTextReset.getLocalBounds();
    buttonTextReset.setOrigin(sf::Vector2f(textBoundsReset.position.x + textBoundsReset.size.x / 2.0f, textBoundsReset.position.y + textBoundsReset.size.y / 2.0f));

    buttonPos = buttonReset.getPosition();
    buttonSize = buttonReset.getSize();
    buttonTextReset.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextReset.setFillColor(sf::Color::White);
       
    if(!backgroundTexture.loadFromFile(settings["settings_background"].get<std::string>()))
    {
        throw std::runtime_error("Failed to load texture file: " + std::string("./images/settings_background.png"));
    }

    backgroundTexture.setSmooth(true);
    backgroundSprite.setTexture(backgroundTexture, true);

    sf::Vector2u textureSize = backgroundTexture.getSize();

    float scaleX = static_cast<float>(800) / textureSize.x;
    float scaleY = static_cast<float>(800) / textureSize.y;
    backgroundSprite.setScale(sf::Vector2f(scaleX, scaleY));

    sf::Vector2u textureSizeSettings = backgroundTexture.getSize();
    float scaleXSettings = static_cast<float>(800) / textureSizeSettings.x;
    float scaleYSettings = static_cast<float>(800) / textureSizeSettings.y;
    backgroundSprite.setScale(sf::Vector2f(scaleXSettings, scaleYSettings));
}

void settingsWin::drawBox() {
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
    
    file >> settings;
    int index  = settings["UserOptions"]["board_style_index"];
    

    for (size_t i = 0; i < boardOptions.size(); ++i) {
        sf::RectangleShape box(sf::Vector2f(200, 30));
        if(index == i) {
            box.setFillColor(sf::Color(100, 100, 200)); 
        } else {
            box.setFillColor(sf::Color(150, 150, 250));
        }
        box.setPosition(sf::Vector2f((width - 200) / 2.f, 80 + i * 30)); 
        optionBoxes.push_back(box);

        sf::Text text(font, load_string(boardOptions[i]), 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f((width - 195) / 2.f, 85 + i * 30));
        optionTexts.push_back(text);
    }
}    
    