#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include "Board.h"
#include <array>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>
#include "loadString.h"
#include "enum.h"
#include <nlohmann/json.hpp> 

using json = nlohmann::json;

class settingsWin {
private:

	json settings;
	int width;
	int height;

	sf::RectangleShape buttonBack; 
	sf::Text buttonTextBack;

	sf::Font font; 

	sf::RectangleShape buttonReset; 
	sf::Text buttonTextReset;

	sf::RectangleShape applyChangesButton;
	sf::Text buttonTextApplyChanges;


	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 

	sf::RectangleShape selectBox;
	sf::Text selectBoxText;
	sf::Text selectedText;
	int selectedIndex;

	std::vector<sf::RectangleShape> optionBoxes;
    std::vector<sf::Text> optionTexts;

	std::vector<Strings> boardOptions = {BLACK_WHITE, LIGHT_WOOD, DARK_WOOD, BLUE_GRAY, GREEN_MARBLE, SLATE_DARK};

public:
	settingsWin();
	sf::RectangleShape getButtonBack() { return buttonBack; } 
	sf::Text getButtonTextBack() { return buttonTextBack; } 
	sf::RectangleShape getButtonReset() { return buttonReset; }
	sf::Text getButtonTextReset() { return buttonTextReset; } 
	sf::Sprite getBackgroundSprite() { return backgroundSprite; }
	sf::RectangleShape getApplyChangesButton() { return applyChangesButton; }
	sf::Text getButtonTextApplyChanges() { return buttonTextApplyChanges; }	

	sf::RectangleShape getSelectBox() { return selectBox; }
	sf::Text getSelectedText() { return selectedText; }
	sf::Text getSelectBoxText() { return selectBoxText; }
	std::vector<sf::RectangleShape>& getOptionBoxes() { return optionBoxes; }
	std::vector<sf::Text> getOptionTexts() { return optionTexts; }
	int getSelectedIndex() const { return selectedIndex; }
	void setSelectedText(const sf::Text& text) { selectedText = text; }
	void setSelectedIndex(int index) { selectedIndex = index; }
	void drawBox();

};