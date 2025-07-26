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


struct chessPiece {
	sf::Sprite Sprite; // prikazuje vizualni prikaz figure
	Figure pieceID;
	int x, y; // ID figure, x i y koordinate figure
	bool draw = 0; // jel figuru treba crtati ili ne 
	chessPiece() ;
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

