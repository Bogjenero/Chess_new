#include "boardWin.h"
#include "Iscrtavanje.h"
#include <SFML/Graphics.hpp>
#include <array>
#include "Board.h"
#include <string>
#include <iostream>
#include "enum.h"


boardWin::boardWin() : boardSprite(emptyTexture), boardTextture(emptyTexture) {}   



void boardWin::RemovePieceAt(const Point& position)
{
    
    for (int i = 0; i < 64; ++i)
    {

        if (chessPieces[i].draw == 1)
        {
            
            if (chessPieces[i].x == position.x && chessPieces[i].y == position.y)
            {
                
                chessPieces[i].draw = 0;

                
                chessPieces[i].Sprite.setPosition(sf::Vector2f(
                    Holder.position.x + (chessPieces[i].x * Holder.size.x / 8),
                    Holder.position.y + (chessPieces[i].y * Holder.size.y / 8)
                ));
                chessPieces[i].Sprite.setScale(sf::Vector2f(Holder.size.x / 1600.f, Holder.size.y / 1600.f));
                return;
            }
        }
    }
}
void boardWin::MapPieces()
{
    for (int i = 0; i < 64; ++i)
    {

        if (chessPieces[i].draw == 1)
        {
            
            chessPieces[i].Sprite.setPosition(sf::Vector2f(Holder.position.x + (chessPieces[i].x * Holder.size.x / 8), Holder.position.y + (chessPieces[i].y * Holder.size.y / 8)));
            chessPieces[i].Sprite.setScale(sf::Vector2f(Holder.size.x / 1600.f, Holder.size.y / 1600.f));
        }
    }
}
void boardWin::MapPieces(move curr)
{
    chessPiece* current = nullptr;
    bool capture = false;
    for (int i = 0; i < 64; ++i)
    {
        if (chessPieces[i].draw == 1)
        {
            if (chessPieces[i].x == curr.from.x && chessPieces[i].y == curr.from.y)
            {
                current = &chessPieces[i];
            }
            if (chessPieces[i].x == curr.to.x && chessPieces[i].y == curr.to.y)
            {
                chessPieces[i].draw = 0;
                capture = true;
                }
                chessPieces[i].Sprite.setPosition(sf::Vector2f(Holder.position.x + (chessPieces[i].x * Holder.size.x / 8), Holder.position.y + (chessPieces[i].y * Holder.size.y / 8)));
                chessPieces[i].Sprite.setScale(sf::Vector2f(Holder.size.x / 1600.f, Holder.size.y / 1600.f));
            }
        }
        current->x = curr.to.x;
        current->y = curr.to.y;

        current->Sprite.setPosition(sf::Vector2f(Holder.position.x + (current->x * Holder.size.x / 8), Holder.position.y + (current->y * Holder.size.y / 8)));
        current->Sprite.setScale(sf::Vector2f(Holder.size.x / 1600.f, Holder.size.y / 1600.f));
}

void boardWin::FitToHolder()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            boardSquares[i][j].setPosition(sf::Vector2f(Holder.position.x + (i * Holder.size.x / 8), Holder.position.y + (j * Holder.size.y / 8)));
            boardSquares[i][j].setSize(sf::Vector2f(Holder.size.x / 8, Holder.size.y / 8));
        }

    }
}