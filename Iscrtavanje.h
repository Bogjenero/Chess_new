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
#include "settingsWin.h"
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>
#include "enum.h"
#include "boardWin.h"
#include "StockFish.h"
#include "loadString.h"
#include <nlohmann/json.hpp> 

using json = nlohmann::json;
extern sf::Texture emptyTexture; 

std::wstring  load_string(Strings uID);





class chessWin {
	private:
		settingsWin settingsWindow;
		boardWin boardWindow; 
		StockFish stockfish; 
		nlohmann::json settings;

		sf::RenderWindow win; 
		
		
		sf::RectangleShape buttonStart; // button
		sf::Text buttonTextStart;
		
		sf::RectangleShape buttonQuit;
		sf::Text buttonTextQuit; 

		sf::RectangleShape buttonSettings;
		sf::Text buttonTextSettings; 

		sf::RectangleShape buttonEngine;
		sf::Text buttonTextEngine;

		sf::Texture backgroundTexture;
		sf::Sprite backgroundSprite; 
		
		sf::RectangleShape buttonWhite;
		sf::Text buttonTextWhite;

		sf::RectangleShape buttonBlack;
		sf::Text buttonTextBlack;
		sf::Text colorSelectionTitle;	

		sf::Font font; // font za tekst
		GameState state;
		Strings stringID; // ID stringa
    
		bool playingAgainstAI = false; 
		Figure::Colors humanColor = Figure::white; 
		
		void handleResized(); // Nova funkcija za rukovanje promjenom veličine prozora
		void handleMouseButtonPressed(std::optional<sf::Event>& event); // Nova funkcija za rukovanje pritiskom tipke miša
		void handleClosed(); // Nova funkcija za rukovanje zatvaranjem prozora
		void showEndWindow();
		void resetGame();
		void drawVictoryWindow(Figure::Colors turn);
		void showColorSelection();
		void DrawSquares(); // crta kvadrate na šahovskoj ploči
		void DrawPieces(); // crta figure
		void selectFigures(int projX, int projY);
		void deselectFigures(int projX, int projY);
		bool isAITurn() const; 
		void playAiMove(); 
		void handleMove(move m, std::array<int,4> replace, bool& end, bool rotation, bool passant, Point enPassantPawn);
	public:
		chessBoard cBoard; //šahovska ploča
		bool Update();// rukuje događajima koji su izvedeni na prozoru
		chessWin( int width, int height, std::wstring name, const std::string imgPath[12]); 
		chessWin();			
		void startGameWithAI(Figure::Colors humanColor);
		
};

