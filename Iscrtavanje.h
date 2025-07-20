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
//#include <windows.h>
#include <array>
//#include "imgui.h"
//#include "imgui-SFML.h"
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>



extern sf::Texture emptyTexture; 

struct chessPiece {
	sf::Sprite Sprite; // prikazuje vizualni prikaz figure
	Figure pieceID;
	int x, y; // ID figure, x i y koordinate figure
	bool draw = 0; // jel figuru treba crtati ili ne 
	chessPiece() ;
};

enum class GameState { StartScreen, ChessBoard, Settings,ColorSelection, AgainstEngine };


enum  Strings {
    START,
    VICTORY,
    WHITE_WINS,
    BLACK_WINS,
    DRAW,
    OK,
    CHESS,
    FINISH,
    ENDWINDOW,
	SETTINGS,
	QUIT,
	BACK,
	RESET,
	ENGINE,
	COLOR_SELECTION,
	CHOOSE_WHITE,
	CHOOSE_BLACK, 
	BLACK_WHITE,
	LIGHT_WOOD,
	DARK_WOOD,
	BLUE_GRAY,
	GREEN_MARBLE,
	SLATE_DARK
};
std::wstring  load_string(Strings uID);



class settingsWin {
private:
	
	sf::RectangleShape buttonBack; 
	sf::Text buttonTextBack;

	sf::Font font; 

	sf::RectangleShape buttonReset; 
	sf::Text buttonTextReset;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite; 

	sf::RectangleShape selectBox;
	sf::Text selectBoxText;
	sf::Text selectedText;

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
	
	sf::RectangleShape getSelectBox() { return selectBox; }
	sf::Text getSelectedText() { return selectedText; }
	sf::Text getSelectBoxText() { return selectBoxText; }
	std::vector<sf::RectangleShape> getOptionBoxes() { return optionBoxes; }
	std::vector<sf::Text> getOptionTexts() { return optionTexts; }

};
class boardWin {
private:

		sf::RectangleShape boardSquares[8][8]; //šahovska ploča
		sf::IntRect Holder; // pravokutnik za šahovsku ploča
		std::array<sf::Color,2> fieldColors = { sf::Color(118, 150, 86) ,sf::Color(255, 255, 255) }; // boje za polja // MORA BITI ARRY NE CEOVSKO POLJE
		sf::Texture pieceTex[12]; // figure (bijele,crne)
		chessPiece chessPieces[64]; // figure
		std::array<int,2> selected; // odabrana figura
		int selectedFigures = 0; // jel odabrana figura
		int sX, sY; // x,y koordinate
		sf::Texture boardTextture;
		sf::Sprite boardSprite; 

public: 
		void FitToHolder(); // prilagođava veličinu i poziciju kvadrata na šahovskoj ploči prema veličini pravokutnika
		void MapPieces(move curr); // postavlja figure na šahovsku ploču prema zadnjem potezu
		void MapPieces(); // ažurira pozicije svih figura na početku igre.
		chessPiece& getChessPieceAt(int i) { return chessPieces[i]; } 
		void setChessPieceXY(int i, int x, int y) { chessPieces[i].x = x; chessPieces[i].y = y; }
		void setChessPieceDraw(int i, int draw) { chessPieces[i].draw = draw; }
		void setChessPiecesFigure(int i, Figure::Figures figure) { chessPieces[i].pieceID.figure = figure; }
		void setChessPiecesColor(int i, Figure::Colors color) { chessPieces[i].pieceID.color = color; }
		
		void setChessPieceSprite(int i, const sf::Sprite& sprite) { chessPieces[i].Sprite = sprite; }
		void setChessPieceTexture(int i, const sf::Texture& texture) { chessPieces[i].Sprite.setTexture(texture,true); }
		sf::Texture& getPieceTexture(int text) {return pieceTex[text];	}

		sf::Texture getBoardTexture() { return boardTextture; }
		sf::Sprite getBoardSprite() { return boardSprite; }
		void setBoardSpriteTexture(const sf::Texture& texture) { boardSprite.setTexture(texture,true); }
		void setBoardSpritePosition(const sf::Vector2f& position) { boardSprite.setPosition(position); }
		void boardSpriteSetScale(const sf::Vector2f& scale) { boardSprite.setScale(scale); }

		sf::RectangleShape& getBoardSquareAt(int i, int j) { return boardSquares[i][j]; } 
		void RemovePieceAt(const Point& position);
		void setSX(int x) { sX = x; }
		void setSY(int y) { sY = y; }
		int getSX() { return sX; }
		int getSY() { return sY; }
		void setHolderPosition(int x, int y) { Holder.position.x = x; Holder.position.y = y; }
		void setHolderSize(int x, int y) { Holder.size.x = x; Holder.size.y = y; }
		sf::IntRect getHolder() { return Holder; }
		int getHolderX() { return Holder.position.x; }
		int getHolderY() { return Holder.position.y; }
		int getSelectedX() { return selected[0]; }
		int getSelectedY() { return selected[1]; }
		void setSelected (int x,int y) { selected[0] = x; selected[1] = y; }
		void setBoardSquareAt(int i, int j, const sf::RectangleShape& square) { boardSquares[i][j] = square; }
		std::array<sf::Color, 2> getFieldColors() { return fieldColors; }
		void setFieldColors(const std::array<sf::Color, 2>& colors) { fieldColors = colors; }
		int getSelectedFigures() { return selectedFigures; }
		void setSelectedFigures(int figures) { selectedFigures = figures; }
		boardWin();
};


class StockFish {
private:
	int toStockfish[2];     // pipe za slanje podataka Stockfishu 
    int fromStockfish[2];   // pipe za čitanje podataka od Stockfisha 
	pid_t pid;
	//FILE* stockFish;
public:
	StockFish();
	void sendCommand(const std::string& command); // šalje naredbu Stockfishu
	std::string getResponse(); // čita odgovor od Stockfisha
	std::string getBestMove(const std::string& fen); // dobiva najbolji potez od Stockfisha
};



class chessWin {
	private:
		settingsWin settingsWindow;
		boardWin boardWindow; 
		StockFish stockfish; 

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

