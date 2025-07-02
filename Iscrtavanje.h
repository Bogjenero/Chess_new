#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include "Board.h"
#include <windows.h>
#include <array>

struct chessPiece {
	sf::Sprite Sprite; // prikazuje vizualni prikaz figure
	Figure pieceID;
	int x, y; // ID figure, x i y koordinate figure
	bool draw = 0; // jel figuru treba crtati ili ne 
};

enum class GameState { StartScreen, ChessBoard };

class chessWin {
private:
	
	sf::RenderWindow win; // prozor
	sf::RectangleShape button; // button
	sf::RectangleShape boardSquares[8][8]; //šahovska ploča
	sf::IntRect Holder; // pravokutnik za šahovsku ploča
	std::array<sf::Color,2> fieldColors = { sf::Color(118, 150, 86) ,sf::Color(255, 255, 255) }; // boje za polja // MORA BITI ARRY NE CEOVSKO POLJE
	sf::Texture pieceTex[12]; // figure (bijele,crne)
	chessPiece chessPieces[64]; // figure
	std::array<int,2> selected; // odabrana figura
	int selectedFigures = 0; // jel odabrana figura
	int sX, sY; // x,y koordinate
	sf::Text buttonText; // tekst na gumbu
	sf::Font font; // font za tekst
	GameState state;

	void FitToHolder(); // prilagođava veličinu i poziciju kvadrata na šahovskoj ploči prema veličini pravokutnika
	void DrawSquares(); // crta kvadrate na šahovskoj ploči
	void DrawPieces(); // crta figure
	void MapPieces(move curr); // postavlja figure na šahovsku ploču prema zadnjem potezu
	void MapPieces(); // ažurira pozicije svih figura na početku igre.
	
	void handleResized(); // Nova funkcija za rukovanje promjenom veličine prozora
	void handleMouseButtonPressed(sf::Event& event); // Nova funkcija za rukovanje pritiskom tipke miša
	void handleClosed(); // Nova funkcija za rukovanje zatvaranjem prozora
	void showEndWindow();
	void resetGame();
	void drawVictoryWindow(Figure::Colors turn);
	void RemovePieceAt(const Point& position);
public:
	chessBoard cBoard; //šahovska ploča
	bool Update();// rukuje događajima koji su izvedeni na prozoru
	chessWin(int width, int height, std::wstring name, const std::string imgPath[12]);
	chessWin();
	std::wstring load_string(UINT uID);
};

