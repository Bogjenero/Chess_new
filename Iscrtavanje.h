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
#include "imgui.h"
#include "imgui-SFML.h"




extern sf::Texture emptyTexture; 

struct chessPiece {
	sf::Sprite Sprite; // prikazuje vizualni prikaz figure
	Figure pieceID;
	int x, y; // ID figure, x i y koordinate figure
	bool draw = 0; // jel figuru treba crtati ili ne 
	chessPiece() ;
};

enum class GameState { StartScreen, ChessBoard, Settings };


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
    //WHITE_WON = WHITE_WINS, 
    //BLACK_WON = BLACK_WINS,
	QUIT
};

class chessWin {
private:
	
	sf::RenderWindow win; // prozor
	
	sf::RectangleShape buttonStart; // button
	sf::Text buttonTextStart;
	 
	sf::RectangleShape buttonQuit;
	sf::Text buttonTextQuit; // tekst na gumbu

	sf::RectangleShape buttonSettings;
	sf::Text buttonTextSettings; // tekst na gumbu 

	sf::Texture boardTextture;
	sf::Sprite boardSprite; 
	
	sf::Texture backgroundTextureStart;
	sf::Sprite backgroundSpriteStart; 

	sf::Texture backgroundTextureSettings;
	sf::Sprite backgroundSpriteSettings;





	sf::RectangleShape boardSquares[8][8]; //šahovska ploča
	sf::IntRect Holder; // pravokutnik za šahovsku ploča
	std::array<sf::Color,2> fieldColors = { sf::Color(118, 150, 86) ,sf::Color(255, 255, 255) }; // boje za polja // MORA BITI ARRY NE CEOVSKO POLJE
	sf::Texture pieceTex[12]; // figure (bijele,crne)
	chessPiece chessPieces[64]; // figure
	std::array<int,2> selected; // odabrana figura
	int selectedFigures = 0; // jel odabrana figura
	int sX, sY; // x,y koordinate
	
	sf::Font font; // font za tekst
	GameState state;
	Strings stringID; // ID stringa

	void FitToHolder(); // prilagođava veličinu i poziciju kvadrata na šahovskoj ploči prema veličini pravokutnika
	void DrawSquares(); // crta kvadrate na šahovskoj ploči
	void DrawPieces(); // crta figure
	void MapPieces(move curr); // postavlja figure na šahovsku ploču prema zadnjem potezu
	void MapPieces(); // ažurira pozicije svih figura na početku igre.
	
	void handleResized(); // Nova funkcija za rukovanje promjenom veličine prozora
	void handleMouseButtonPressed(std::optional<sf::Event>& event); // Nova funkcija za rukovanje pritiskom tipke miša
	void handleClosed(); // Nova funkcija za rukovanje zatvaranjem prozora
	void showEndWindow();
	void resetGame();
	void drawVictoryWindow(Figure::Colors turn);
	void RemovePieceAt(const Point& position);
	static const  std::map<Strings, std::wstring> stringMap; 
	
public:
	chessBoard cBoard; //šahovska ploča
	bool Update();// rukuje događajima koji su izvedeni na prozoru
	chessWin( int width, int height, std::wstring name, const std::string imgPath[12]);
	chessWin();
	std::wstring load_string(Strings uID);
	//dodati windza settings recimo settingsWin (kao što imam chessWin) gdje će biti stvari za podesiti i onda inicijalizirati u mainu
	//unutar settingsWin će biti sve potrebne funkcije i varijable za podešavanje igre
};

