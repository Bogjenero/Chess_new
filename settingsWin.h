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
using namespace sf;

class settingsWin {
private:

	json settings;
	int width;
	int height;

	RectangleShape buttonBack; 
	Text buttonTextBack;

	Font font;

	RectangleShape buttonReset; 
	Text buttonTextReset;

	RectangleShape applyChangesButton;
	Text buttonTextApplyChanges;

	RectangleShape checkBox;
	RectangleShape check;
	bool isChecked;
	Text muteText;

	Text boardStyleText;

	Texture backgroundTexture;
	Sprite backgroundSprite;

	RectangleShape selectBox;
	Text selectBoxText;
	Text selectedText;
	int selectedIndex;

	std::vector<sf::RectangleShape> optionBoxes;
    std::vector<sf::Text> optionTexts;

	std::vector<Strings> boardOptions = {BLACK_WHITE, LIGHT_WOOD, DARK_WOOD, BLUE_GRAY, GREEN_MARBLE, SLATE_DARK};

public:
	settingsWin();
	RectangleShape getButtonBack() { return buttonBack; } 
	Text getButtonTextBack() { return buttonTextBack; } 
	RectangleShape getButtonReset() { return buttonReset; }
	Text getButtonTextReset() { return buttonTextReset; } 
	Sprite getBackgroundSprite() { return backgroundSprite; }
	RectangleShape getApplyChangesButton() { return applyChangesButton; }
	Text getButtonTextApplyChanges() { return buttonTextApplyChanges; }	
	RectangleShape getCheckBox() { return checkBox; }
	RectangleShape& 	 getCheck() { return check; }
	Text getMuteText() { return muteText; }
	bool getIsChecked() { return isChecked; }
	RectangleShape getSelectBox() { return selectBox; }
	Text getSelectedText() { return selectedText; }
	Text getSelectBoxText() { return selectBoxText; }
	std::vector<sf::RectangleShape>& getOptionBoxes() { return optionBoxes; }
	std::vector<sf::Text> getOptionTexts() { return optionTexts; }
	int getSelectedIndex() const { return selectedIndex; }
	void setSelectedText(const sf::Text& text) { selectedText = text; }
	void setSelectedIndex(int index) { selectedIndex = index; }
	Text getBoardStyleText() { return boardStyleText; }
	void drawBox();
	void setIsChecked(bool checked) { isChecked = checked; }
};