﻿#include "Iscrtavanje.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
//#include "imgui.h"
//#include "imgui-SFML.h"
#include <nlohmann/json.hpp> 
#include <fstream>

using json = nlohmann::json;


const std::map <Strings, std::wstring> stringMap = {

    { START, L"Start Game" },
    { VICTORY, L"Victory!" },
    { WHITE_WINS, L"White Wins!" },
    { BLACK_WINS, L"Black Wins!" },
    { DRAW, L"Draw!" },
    { OK, L"OK" },
    { CHESS, L"Chess Game" },
    { FINISH, L"Finish the game" },
    { ENDWINDOW, L"End window" },
    { QUIT, L"Quit Game"},
    { SETTINGS, L"Settings" },
    { BACK, L"Back to Menu" },
    { RESET, L"Reset Game" }
};


std::wstring load_string(Strings uID) {

    static const std::wstring emptyString = L"";
    auto it = stringMap.find(uID);
    return (it != stringMap.end()) ? it->second : emptyString;

}

chessWin::chessWin(): buttonTextStart( font, load_string(START), 30 ), buttonTextQuit(font,load_string(QUIT),30),buttonTextSettings(font,load_string(SETTINGS),30), 
boardSprite(boardTextture), backgroundSprite(backgroundTexture) {}
sf::Texture emptyTexture; 

chessPiece::chessPiece() : Sprite(emptyTexture) {}


/*const std::string defaultTheme[12] = {
                                "./images/Pieces/Default/wp.png",
                            //"./images/Pieces/Default/wr.png",
                            ".images/Rook.xcf",
                            "./images/Pieces/Default/wn.png",
                            "./images/Pieces/Default/wb.png",
                            "./images/Pieces/Default/wk.png",
                            "./images/Pieces/Default/wq.png",
                            "./images/Pieces/Default/bp.png",
                            "./images/Pieces/Default/br.png",
                            "./images/Pieces/Default/bn.png",
                            "./images/Pieces/Default/bb.png",
                            "./images/Pieces/Default/bk.png",
                            "./images/Pieces/Default/bq.png" };*/


void chessWin::FitToHolder()
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
void chessWin::DrawSquares()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            win.draw(boardSquares[i][j]);
        }
    }
}
void chessWin::DrawPieces()
{
    for (int i = 0; i < 64; ++i)
    {
        if (chessPieces[i].draw == 1)
        {
            win.draw(chessPieces[i].Sprite);
        }
    }
}
void chessWin::MapPieces()
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
void chessWin::MapPieces(move curr)
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
void chessWin::RemovePieceAt(const Point& position)
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


int setTexture(Figure currFigure)
{
    if (currFigure.color == Figure::white)
    {
        if (currFigure.figure == Figure::Pawn)
        {
            return 0;
        }
        else if (currFigure.figure == Figure::Rook)
        {
            return 1;
        }
        else if (currFigure.figure == Figure::Knight)
        {
            return 2;
        }
        else if (currFigure.figure == Figure::King)
        {
            return 4;
        }
        else if (currFigure.figure == Figure::Bishop)
        {
            return 3;
        }
        else if (currFigure.figure == Figure::Queen)
        {
            return 5;
        }
        else {
            return -1; 
        }
    }
    else {
        if (currFigure.figure == Figure::Pawn)
        {
            return 6;
        }
        else if (currFigure.figure == Figure::Rook)
        {
            return 7;
        }
        else if (currFigure.figure == Figure::Knight)
        {
            return 8;
        }
        else if (currFigure.figure == Figure::King)
        {
            return 10;
        }
        else if (currFigure.figure == Figure::Bishop)
        {
            return 9;
        }
        else if (currFigure.figure == Figure::Queen)
        {
            return 11;
        }

        else {
            return -1; 
        }

    }
}
settingsWin::settingsWin() : buttonTextBack(font, load_string(BACK), 30), 
buttonBack(sf::Vector2f(200, 60)), buttonTextReset(font, load_string(RESET), 30), buttonReset(sf::Vector2f(200, 60)),
backgroundSprite(backgroundTexture) 
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

    int height = settings["window"]["height"].get<int>();
    int width = settings["window"]["width"].get<int>();
        
    
    buttonReset.setPosition(sf::Vector2f((width - 200) / 2.f, 200));
    buttonReset.setFillColor(sf::Color::Green);
    buttonReset.setOutlineThickness(2);

    
    buttonBack.setPosition(sf::Vector2f((800 - 200) / 2.f, 300));
    buttonBack.setFillColor(sf::Color::Blue);
    buttonBack.setOutlineThickness(2);

   
    
    sf::FloatRect textBounds = buttonTextBack.getLocalBounds();
    buttonTextBack.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    
    sf::Vector2f buttonPos = buttonBack.getPosition();
    sf::Vector2f buttonSize = buttonBack.getSize();
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

chessWin::chessWin(int width,  int height, std::wstring name, const std::string imgPath[12] ) : buttonTextStart(font, load_string(START), 30), buttonTextQuit(font,load_string(QUIT),30), buttonTextSettings(font,load_string(SETTINGS),30),
 boardSprite(boardTextture), backgroundSprite(backgroundTexture)
{
    
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
    nlohmann::json settings;
    file >> settings;

    sX = settings["window"]["width"].get<int>();
    sY = settings["window"]["height"].get<int>();
    
    
    state = GameState::StartScreen;
    //sX = width;
    //sY = height;
    Holder.position.x = 0;
    Holder.position.y = 0;
    //Holder.size.x = width;
    //Holder.size.y = height;

    Holder.size.x = settings["window"]["width"].get<int>();
    Holder.size.y = settings["window"]["height"].get<int>();
      
    buttonStart.setSize(sf::Vector2f(200, 60));
    buttonStart.setPosition(sf::Vector2f((800 - 200) / 2.f, 100));
    buttonStart.setFillColor(sf::Color::Blue);
    buttonStart.setOutlineThickness(2); 
    
    buttonSettings.setSize(sf::Vector2f(200, 60));
    buttonSettings.setPosition(sf::Vector2f((800 - 200) / 2.f, 200));
    buttonSettings.setFillColor(sf::Color::Green);
    buttonSettings.setOutlineThickness(2);


    buttonQuit.setSize(sf::Vector2f(200, 60));
    buttonQuit.setPosition(sf::Vector2f((800 - 200) / 2.f, 300));
    buttonQuit.setFillColor(sf::Color::Red);
    buttonQuit.setOutlineThickness(2);

    if (!font.openFromFile("arial.ttf")) {
       throw std::runtime_error("Failed to load texture file: " + std::string("arial.ttf"));
    }
    sf::FloatRect textBounds = buttonTextStart.getLocalBounds();
    buttonTextStart.setOrigin(sf::Vector2f( textBounds.position.x + textBounds.size.x / 2.0f,textBounds.position.y + textBounds.size.y / 2.0f));
    
    sf::Vector2f buttonPos = buttonStart.getPosition();
    sf::Vector2f buttonSize = buttonStart.getSize();
    buttonTextStart.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f,buttonPos.y + buttonSize.y / 2.0f));
    buttonTextStart.setFillColor(sf::Color::White);
    
    textBounds = buttonTextQuit.getLocalBounds();
    buttonTextQuit.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));

    buttonPos = buttonQuit.getPosition();
    buttonSize = buttonQuit.getSize();
    buttonTextQuit.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextQuit.setFillColor(sf::Color::White);

    textBounds = buttonTextSettings.getLocalBounds();
    buttonTextSettings.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));

    buttonPos = buttonSettings.getPosition();
    buttonSize = buttonSettings.getSize();
    buttonTextSettings.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextSettings.setFillColor(sf::Color::White);
    
    if(!boardTextture.loadFromFile(settings["boards"]["purple"].get<std::string>()))
    {
        throw std::runtime_error("Failed to load texture file: " + std::string("./images/board0.png"));
    }    

    boardTextture.setSmooth(true);
    boardSprite.setTexture(boardTextture, true);
    boardSprite.setPosition(sf::Vector2f(Holder.position.x, Holder.position.y));

    boardSprite.setScale(sf::Vector2f(
    sX / boardSprite.getLocalBounds().size.x,
    sY / boardSprite.getLocalBounds().size.y));


    if(!backgroundTexture.loadFromFile(settings["start_background"].get<std::string>()))
    {
        throw std::runtime_error("Failed to load texture file: " + std::string("./images/background.png"));
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


    
    /*for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int colorIndex = (i + j) % 2;
            boardSquares[i][j].setFillColor(fieldColors[colorIndex]);
        }
        
    }
    
    FitToHolder();*/
    std::array  figures = {"pawn", "rook", "knight", "bishop", "king", "queen" };
    for (int i = 0 ; i < 12; ++i)
    {
        pieceTex[i].loadFromFile(settings["figures"]["modern"][(i < 6) ? "white" : "black"][figures[i % 6]].get<std::string>());
        pieceTex[i].setSmooth(true);
    }
    /*std::string filePath = settings["figures"]["modern"]["white"]["pawn"].get<std::string>();
    sf::IntRect blank;
    for (int i = 0; i < 12; ++i)
    {
        
        if(pieceTex[i].loadFromFile(settings["figures"]["modern"].get<std::string>()))
        {
            pieceTex[i].setSmooth(true);
        }
        else
        {
            throw std::runtime_error("Failed to load texture file: " + imgPath[i]);
        }

    }*/
    int index = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Figure currFigure = cBoard.chessBoard.arr[i][j];
            
            chessPieces[index].pieceID.figure = currFigure.figure;
            chessPieces[index].pieceID.color = currFigure.color;
            chessPieces[index].x = i;
            chessPieces[index].y = j;
            if (currFigure.figure != Figure::Empty)
            {
                int textureIndex = setTexture(currFigure);
                chessPieces[index].Sprite.setTexture(pieceTex[textureIndex], true);
                chessPieces[index].draw = 1;
            }
            ++index;
        }
    }
    MapPieces();
    
    win.create(sf::VideoMode(sf::Vector2u(width, height)), name );
    
}

void chessWin::handleResized() {
    sX = win.getSize().x;
    sY = win.getSize().y;
    win.setView(sf::View(sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f( sX, sY) )));
    if (sX > sY) {
        Holder.size.x = sY;
        Holder.size.y = sY;
        Holder.position.x = sX / 2 - Holder.size.x / 2;
        Holder.position.y = 0;
    }
    else {
        Holder.size.x = sX;
        Holder.size.y = sX;
        Holder.position.y = sY / 2 - Holder.size.y / 2;
        Holder.position.x = 0;
    }
    MapPieces();
    FitToHolder();
}

void chessWin::handleMouseButtonPressed(std::optional<sf::Event>& event) {
    const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
  
    if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(win);
        if (state == GameState::StartScreen) {
            if (buttonStart.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::ChessBoard;
                
                MapPieces();
            }
            else if (buttonSettings.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::Settings;
            }
            else if (buttonQuit.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                win.close();
            }
        }
        else if (state == GameState::ChessBoard) {
            int pX = mouseButtonPressed->position.x;
            int pY = mouseButtonPressed->position.y;
            int projX = ((pX - Holder.position.x) - ((pX - Holder.position.x) % (Holder.size.x / 8))) / (Holder.size.x / 8);
            int projY = ((pY - Holder.position.y) - ((pY - Holder.position.y) % (Holder.size.y / 8))) / (Holder.size.y / 8);
            if (selectedFigures == 0) {
                if (pX >= Holder.position.x && pX <= Holder.position.x + Holder.size.x && pY > Holder.position.y && pY < Holder.position.y + Holder.size.y) {
                    selected[0] = projX;
                    selected[1] = projY;
                    boardSquares[projX][projY].setFillColor(sf::Color(186, 202, 68));
                    selectedFigures = 1;
                }
            }
            else {
                if (selected[0] == projX && selected[1] == projY) {
                    boardSquares[projX][projY].setFillColor(fieldColors[ ((projX + projY) % 2)]);
                    selectedFigures = 0;
                }
                else {

                    move m(Point(selected[0], selected[1]),Point( projX, projY));
                    std::array<int, 4> replace = { 0,0,0,0 };
                    bool rotation = false;
                    bool end = false;
                    bool Passant = false;
                    Point enPassantPawn;
                    if (cBoard.playMove(m,replace,end,rotation,enPassantPawn,Passant)) {
                        if(rotation)
						{
							  MapPieces(m);
                              move m2 = move(Point(replace[0], replace[1]),Point( replace[2], replace[3]));
                              MapPieces(m2);
                              cBoard.nextTurn();
						}
                        else if (Passant) {
                            MapPieces(m);
                            RemovePieceAt(enPassantPawn);
                            cBoard.nextTurn();
                        }
                        else {
                            MapPieces(m);
                            if (end) {
                                DrawSquares();
                                DrawPieces();
                                win.display();
                                drawVictoryWindow(cBoard.turn);
                            }
                            else {
                                cBoard.nextTurn();
                            }
                        }
                    }
                    boardSquares[selected[0]][selected[1]].setFillColor(fieldColors[ ((selected[0] + selected[1]) % 2)]);
                    selectedFigures = 0;
                }
            }
        }
        else if (state == GameState::Settings) {
            if (settingsWindow.getButtonBack().getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::StartScreen;
            }
            else if (settingsWindow.getButtonReset().getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                //resetGame();
                // ovjde moram napraviti da postavke budu resetirane
            }
        }
    }
    else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
        boardSquares[selected[0]][selected[1]].setFillColor(fieldColors[((selected[0] + selected[1]) % 2)]);
        selectedFigures = 0;
    }

}

void chessWin::handleClosed() {
    win.close();
}


void chessWin::resetGame() {
    cBoard.chessBoard.arr[0][0] = { Figure::Rook, Figure::black };
    cBoard.chessBoard.arr[1][0] = { Figure::Knight, Figure::black };
    cBoard.chessBoard.arr[2][0] = { Figure::Bishop, Figure::black };
    cBoard.chessBoard.arr[3][0] = { Figure::Queen, Figure::black };
    cBoard.chessBoard.arr[4][0] = { Figure::King, Figure::black };
    cBoard.chessBoard.arr[5][0] = { Figure::Bishop, Figure::black };
    cBoard.chessBoard.arr[6][0] = { Figure::Knight, Figure::black };
    cBoard.chessBoard.arr[7][0] = { Figure::Rook, Figure::black };

    for (int i = 0; i < 8; ++i) {
        cBoard.chessBoard.arr[i][1] = { Figure::Pawn, Figure::black };
    }
    cBoard.chessBoard.arr[0][7] = { Figure::Rook, Figure::white };
    cBoard.chessBoard.arr[1][7] = { Figure::Knight, Figure::white };
    cBoard.chessBoard.arr[2][7] = { Figure::Bishop, Figure::white };
    cBoard.chessBoard.arr[3][7] = { Figure::Queen, Figure::white };
    cBoard.chessBoard.arr[4][7] = { Figure::King, Figure::white };
    cBoard.chessBoard.arr[5][7] = { Figure::Bishop, Figure::white };
    cBoard.chessBoard.arr[6][7] = { Figure::Knight, Figure::white };
    cBoard.chessBoard.arr[7][7] = { Figure::Rook, Figure::white };

    for (int i = 0; i < 8; ++i) {
        cBoard.chessBoard.arr[i][6] = { Figure::Pawn, Figure::white };
    }
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            cBoard.chessBoard.arr[j][i] = { Figure::Empty, Figure::none };
        }
    }
    int index = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Figure currFigure = cBoard.chessBoard.arr[i][j];

            chessPieces[index].pieceID.figure = currFigure.figure;
            chessPieces[index].pieceID.color = currFigure.color;
            chessPieces[index].x = i;
            chessPieces[index].y = j;
            if (currFigure.figure != Figure::Empty) {
                int textureIndex = setTexture(currFigure);
                chessPieces[index].Sprite.setTexture(pieceTex[textureIndex], true);
                chessPieces[index].draw = 1;
            }
            else {
                chessPieces[index].draw = 0;
            }
            ++index;
        }
    }
    cBoard.bKing_moved = false, cBoard.wKing_moved = false, cBoard.bRook1_moved = false, cBoard.bRook2_moved = false, cBoard.wRook1_moved = false, cBoard.wRook2_moved = false;
    MapPieces();
    FitToHolder();
}


void chessWin::showEndWindow()
{
    sf::RenderWindow endWindow(sf::VideoMode(sf::Vector2u(400, 200)), load_string(ENDWINDOW));
  
    win.setActive(false);

    sf::RectangleShape button(sf::Vector2f(250, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(sf::Vector2f(100, 75));

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) 
    {
        throw std::runtime_error("Failed to load texture file: " + std::string("arial.ttf"));
    }

    sf::Text buttonText(font,load_string(FINISH), 24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(sf::Vector2f(150, 85)); 
       
    while (endWindow.isOpen())
    {
        std::optional<sf::Event> event;
       
        while (event = endWindow.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                endWindow.close();
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if(mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(endWindow);
                    if (button.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y) )) {
                        
                        win.close();

                        
                        resetGame();

                       
                        state = GameState::StartScreen;
                        if(cBoard.turn == Figure::black)
						{
							cBoard.nextTurn();
						}   
                        win.create(sf::VideoMode(sf::Vector2u(sX, sY)), load_string(CHESS));
                  
                        FitToHolder();
                        MapPieces();
                        DrawSquares();
                        DrawPieces();

                        
                        return;
                    }
                }
            }
        }
        if (!endWindow.hasFocus())
        {
            endWindow.requestFocus();
        }

        endWindow.clear();
        endWindow.draw(button);
        endWindow.draw(buttonText);
        endWindow.display();
    }
    win.setActive(true);
}


void chessWin::drawVictoryWindow(Figure::Colors turn) 
{
    sf::RenderWindow Victorywindow(sf::VideoMode(sf::Vector2u(400, 200)), load_string(VICTORY));
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load texture file: " + std::string("arial.ttf"));
    }
    sf::Text text(font, load_string((turn == Figure::white) ? WHITE_WINS : BLACK_WINS), 24);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(
        textRect.position.x + textRect.size.x / 2.0f,
        textRect.position.y + textRect.size.y / 2.0f)
    );

    text.setPosition(
        sf::Vector2f(Victorywindow.getSize().x / 2.0f,
        Victorywindow.getSize().y / 2.0f - 20.0f)
    );

    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setOutlineColor(sf::Color::Black);
    button.setOutlineThickness(2);
    button.setPosition(sf::Vector2f(Victorywindow.getSize().x / 2.0f - 50, Victorywindow.getSize().y / 2.0f + 20));

    sf::Text buttonText(font, load_string(OK), 18);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setStyle(sf::Text::Bold);

    
    buttonText.setOrigin(sf::Vector2f(
                        buttonText.getLocalBounds().position.x + buttonText.getLocalBounds().size.x / 2.0f,
                        buttonText.getLocalBounds().position.x + buttonText.getLocalBounds().size.y / 2.0f));
    
    buttonText.setPosition(sf::Vector2f(
                            button.getPosition().x + button.getSize().x / 2.0f,
                            button.getPosition().y + button.getSize().y / 2.0f));

    while (Victorywindow.isOpen()) {
        //sf::Event event;
        std::optional<sf::Event> event;
        while (auto eventOpt =  Victorywindow.pollEvent()) {
            event = eventOpt;
            if (event->is<sf::Event::Closed>())
                Victorywindow.close();
            //else if (event->is == sf::Event::MouseButtonPressed)
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(Victorywindow);
                    if (button.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {

                        win.close();


                        resetGame();

                        state = GameState::StartScreen;
                        win.create(sf::VideoMode(sf::Vector2u(sX, sY)), load_string(CHESS));

                        FitToHolder();
                        MapPieces();
                        DrawSquares();
                        DrawPieces();

                        return;
                    }
                }
            }

        }
        Victorywindow.clear();

        Victorywindow.draw(text);
        Victorywindow.draw(button);
        Victorywindow.draw(buttonText);

        Victorywindow.display();
    }
}

bool chessWin::Update() {
    std::optional<sf::Event> event;
    
    auto cursorArrow = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
    auto cursorHand = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
    
    sf::Cursor handCursor = std::move(*cursorHand);
    sf::Cursor arrowCursor = std::move(*cursorArrow);
    while (event = win.pollEvent()) {
        
        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(win));
        
        if(state == GameState::StartScreen && (buttonStart.getGlobalBounds().contains(mousePos) || buttonQuit.getGlobalBounds().contains(mousePos)  || buttonSettings.getGlobalBounds().contains(mousePos)))
        {
            
            win.setMouseCursor(handCursor);
        }
        else 
        {
            win.setMouseCursor(arrowCursor);
        }
        if(state == GameState::Settings && (settingsWindow.getButtonBack().getGlobalBounds().contains(mousePos) || settingsWindow.getButtonReset().getGlobalBounds().contains(mousePos)))
        {
            win.setMouseCursor(handCursor);
        }
        else if (state == GameState::ChessBoard && selectedFigures == 0) {
            win.setMouseCursor(arrowCursor);
        }
        
        if (event->is<sf::Event::Resized>()) {
            //handleResized();
        }
        else if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {

            handleMouseButtonPressed(event);
        }   

        else if(event->is<sf::Event::Closed>()) {
            handleClosed();
        }

        else if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape && state == GameState::ChessBoard) {
            showEndWindow();
        }
    }
    }

    win.clear();
    if (state == GameState::StartScreen) {
        win.draw(backgroundSprite);
        win.draw(buttonStart);
        win.draw(buttonTextStart);
        win.draw(buttonSettings);
        win.draw(buttonTextSettings);
        win.draw(buttonQuit);
        win.draw(buttonTextQuit);

    }
    else if (state == GameState::ChessBoard) {
        win.draw(boardSprite);
        //DrawSquares();
        DrawPieces();
    }
    else if (state == GameState::Settings) {
        win.draw(settingsWindow.getBackgroundSprite());
        win.draw(settingsWindow.getButtonBack());
        win.draw(settingsWindow.getButtonTextBack());
        win.draw(settingsWindow.getButtonReset());
        win.draw(settingsWindow.getButtonTextReset());
    }

    win.display();
    return true;
}