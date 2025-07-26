#include "Iscrtavanje.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <sys/select.h>
#include <nlohmann/json.hpp> 
#include <fstream>
#include <unistd.h>
#include <limits.h>



// Definition for RGBColor
struct RGBColor {
    int r;
    int g;
    int b;
};

sf::Texture emptyTexture; 

void from_json(const json& j, RGBColor& color) {
    j.at("r").get_to(color.r);
    j.at("g").get_to(color.g);
    j.at("b").get_to(color.b);
}



void chessWin::handleMove(move m, std::array<int,4> replace, bool& end, bool rotation, bool passant, Point enPassantPawn) {
    boardWindow.MapPieces(m);
    if (rotation) {
        move m2(Point(replace[0], replace[1]), Point(replace[2], replace[3]));
        boardWindow.MapPieces(m2);
    }
    if (passant) {
        boardWindow.RemovePieceAt(enPassantPawn);
    }
    if (end) {
        DrawSquares();
        DrawPieces();
        win.display();
        drawVictoryWindow(cBoard.turn);
    } else {
        cBoard.nextTurn();
    }
}







chessWin::chessWin(): buttonTextStart( font, load_string(START), 30 ), buttonTextQuit(font,load_string(QUIT),30),buttonTextSettings(font,load_string(SETTINGS),30), 
 backgroundSprite(backgroundTexture), buttonTextEngine(font, load_string(ENGINE), 30), 
 buttonTextWhite(font, load_string(CHOOSE_WHITE), 30), buttonTextBlack(font, load_string(CHOOSE_BLACK), 30),
 colorSelectionTitle(font, load_string(COLOR_SELECTION), 30) {}


chessPiece::chessPiece() : Sprite(emptyTexture) {}




void chessWin::DrawSquares()
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            win.draw(boardWindow.getBoardSquareAt(i, j));
        }
    }
}
void chessWin::DrawPieces()
{
    int drawnPieces = 0;
    for (int i = 0; i < 64; ++i)
    {
        if (boardWindow.getChessPieceAt(i).draw == 1)
        {
            drawnPieces++;
            win.draw(boardWindow.getChessPieceAt(i).Sprite);
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
 




chessWin::chessWin(int width,  int height, std::wstring name, const std::string imgPath[12] ) : buttonTextStart(font, load_string(START), 30), buttonTextQuit(font,load_string(QUIT),30), buttonTextSettings(font,load_string(SETTINGS),30),
  backgroundSprite(backgroundTexture), buttonTextEngine(font, load_string(ENGINE), 25), 
  buttonTextWhite(font, load_string(WHITE_WINS), 25), buttonTextBlack(font, load_string(BLACK_WINS), 25),
colorSelectionTitle(font, load_string(CHESS), 30)
{

    
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
   
    file >> settings;

    boardWindow.setSX(settings["window"]["width"].get<int>());
    boardWindow.setSY(settings["window"]["height"].get<int>());

    boardWindow.setFieldColors({
        sf::Color(settings["boards"]["slateDark"][0]["r"].get<int>(), settings["boards"]["slateDark"][0]["g"].get<int>(), settings["boards"]["slateDark"][0]["b"].get<int>()),
        sf::Color(settings["boards"]["slateDark"][1]["r"].get<int>(), settings["boards"]["slateDark"][1]["g"].get<int>(), settings["boards"]["slateDark"][1]["b"].get<int>())
    });


    state = GameState::StartScreen;

    boardWindow.setHolderPosition(settings["window"]["width"].get<int>() / 2 - settings["window"]["height"].get<int>() / 2, 0);
    boardWindow.setHolderSize(settings["window"]["height"].get<int>(), settings["window"]["height"].get<int>());

    buttonStart.setSize(sf::Vector2f(200, 60));
    buttonStart.setPosition(sf::Vector2f((800 - 200) / 2.f, 100));
    buttonStart.setFillColor(sf::Color::Blue);
    buttonStart.setOutlineThickness(2); 
    
    buttonEngine.setSize(sf::Vector2f(200, 60));
    buttonEngine.setPosition(sf::Vector2f((800 - 200) / 2.f, 200));
    buttonEngine.setFillColor(sf::Color::Blue);
    buttonEngine.setOutlineThickness(2);


    buttonSettings.setSize(sf::Vector2f(200, 60));
    buttonSettings.setPosition(sf::Vector2f((800 - 200) / 2.f, 300));
    buttonSettings.setFillColor(sf::Color::Green);
    buttonSettings.setOutlineThickness(2);


    buttonQuit.setSize(sf::Vector2f(200, 60));
    buttonQuit.setPosition(sf::Vector2f((800 - 200) / 2.f, 400));
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
    
    buttonPos = buttonEngine.getPosition();
    buttonSize = buttonEngine.getSize();
    buttonTextEngine.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f,buttonPos.y + buttonSize.y / 2.0f));
    buttonTextEngine.setFillColor(sf::Color::White);

    textBounds = buttonTextEngine.getLocalBounds();
    buttonTextEngine.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));

    boardWindow.getBoardTexture().setSmooth(true);
    boardWindow.setBoardSpriteTexture(boardWindow.getBoardTexture());

    boardWindow.getBoardSprite().setPosition(sf::Vector2f(boardWindow.getHolderX(), boardWindow.getHolderY()));
    boardWindow.setBoardSpritePosition(sf::Vector2f(boardWindow.getHolderX(), boardWindow.getHolderY()));
    boardWindow.boardSpriteSetScale(sf::Vector2f(
        boardWindow.getSX() / boardWindow.getBoardSprite().getLocalBounds().size.x,
        boardWindow.getSY() / boardWindow.getBoardSprite().getLocalBounds().size.y
    ));

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


    
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int colorIndex = (i + j) % 2;
            boardWindow.getBoardSquareAt(i, j).setFillColor(boardWindow.getFieldColors()[colorIndex]);
        }
        
    }   
  

   boardWindow.FitToHolder();
    std::array  figures = {"pawn", "rook", "knight", "bishop", "king", "queen" };
    for (int i = 0 ; i < 12; ++i)
    {
        boardWindow.getPieceTexture(i).loadFromFile(settings["figures"]["modern"][(i < 6) ? "white" : "black"][figures[i % 6]].get<std::string>());
        boardWindow.getPieceTexture(i).setSmooth(true);
    }
    
    int index = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Figure currFigure = cBoard.chessBoard.arr[i][j];
            boardWindow.setChessPiecesFigure(index, currFigure.figure);
            boardWindow.setChessPiecesColor(index, currFigure.color);
            boardWindow.setChessPieceXY(index, i, j);

            
            if (currFigure.figure != Figure::Empty)
            {
                    int textureIndex = setTexture(currFigure);
                    if (textureIndex >= 0 && textureIndex < 12) {
                        boardWindow.setChessPieceTexture(index, boardWindow.getPieceTexture(textureIndex)); // This should work better now
                        boardWindow.setChessPieceDraw(index, 1);
                    }
            }
            ++index;
        }
    }
    boardWindow.MapPieces();
    

 win.create(sf::VideoMode(sf::Vector2u(width, height)), name );
    
}

void chessWin::handleResized() {
    boardWindow.setSX(win.getSize().x);
    boardWindow.setSY(win.getSize().y);
    win.setView(sf::View(sf::FloatRect(sf::Vector2f(0,0), sf::Vector2f( boardWindow.getSX(), boardWindow.getSY()) )));
    if (boardWindow.getSX() > boardWindow.getSY()) {
        boardWindow.setHolderSize(boardWindow.getSX(), boardWindow.getSY());
        boardWindow.setHolderPosition(boardWindow.getSX() / 2 - boardWindow.getSY() / 2,0);
    }
    else {
        boardWindow.setHolderSize(boardWindow.getSX(), boardWindow.getSX());
        boardWindow.setHolderPosition(0, boardWindow.getSY() / 2 - boardWindow.getSX() / 2);
    }
    boardWindow.MapPieces();
    boardWindow.FitToHolder();
}

void chessWin::handleMouseButtonPressed(std::optional<sf::Event>& event) {
    const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>();
  
    if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(win);
        if (state == GameState::StartScreen) {
            if (buttonStart.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::ChessBoard;

                    boardWindow.MapPieces();
            }
            else if (buttonSettings.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                settingsWindow.drawBox();
                state = GameState::Settings;
            }
            else if (buttonQuit.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                win.close();
            }
            else if (buttonEngine.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::ColorSelection;                
            }

        }
        else if (state == GameState::ChessBoard) {
             if (!playingAgainstAI || !isAITurn()) {
            int pX = mouseButtonPressed->position.x;
            int pY = mouseButtonPressed->position.y;
            int projX = ((pX - boardWindow.getHolder().position.x) - ((pX - boardWindow.getHolder().position.x) % (boardWindow.getHolder().size.x / 8))) / (boardWindow.getHolder().size.x / 8);
            int projY = ((pY - boardWindow.getHolder().position.y) - ((pY - boardWindow.getHolder().position.y) % (boardWindow.getHolder().size.y / 8))) / (boardWindow.getHolder().size.y / 8);
                if (projX < 0 || projX >= 8 || projY < 0 || projY >= 8) {
                    if (boardWindow.getSelectedFigures() == 1) {
                        boardWindow.getBoardSquareAt(boardWindow.getSelectedX(), boardWindow.getSelectedY()).setFillColor(boardWindow.getFieldColors()[(boardWindow.getSelectedX() + boardWindow.getSelectedY()) % 2]);
                        boardWindow.setSelectedFigures(0);
                    }
                    return; 
                }
            if (boardWindow.getSelectedFigures() == 0) {
                if (cBoard.chessBoard.arr[projX][projY].figure == Figure::Empty || cBoard.chessBoard.arr[projX][projY].color != cBoard.turn) {
                   return;
                }
                else {                
                    selectFigures(projX, projY);
                    boardWindow.setSelectedFigures(1);
                }
            }
            else {
                if (boardWindow.getSelectedX() == projX && boardWindow.getSelectedY() == projY) {
                    deselectFigures(projX, projY);
                    boardWindow.setSelectedFigures(0);
                }
             else if(cBoard.chessBoard.arr[projX][projY].figure != Figure::Empty &&  cBoard.chessBoard.arr[projX][projY].color == cBoard.turn) {
                    deselectFigures(boardWindow.getSelectedX(), boardWindow.getSelectedY());
                    selectFigures(projX, projY);
                    return;
            }
            else {
                    move m(Point(boardWindow.getSelectedX(), boardWindow.getSelectedY()),Point( projX, projY));
                    std::array<int, 4> replace = { 0,0,0,0 };
                    bool rotation = false;
                    bool end = false;
                    bool Passant = false;
                    Point enPassantPawn;
                    if (cBoard.playMove(m,replace,end,rotation,enPassantPawn,Passant)) {
                        handleMove(m, replace, end, rotation, Passant, enPassantPawn);
                    }
                    if (playingAgainstAI && !end && isAITurn()) {
                            playAiMove();
                    }
                    boardWindow.getBoardSquareAt(boardWindow.getSelectedX(), boardWindow.getSelectedY()).setFillColor(boardWindow.getFieldColors()[((boardWindow.getSelectedX() + boardWindow.getSelectedY()) % 2)]);
                    boardWindow.setSelectedFigures(0);
                }
            }
        }
    }
    else if (state == GameState::Settings) {
        const auto& boxes = settingsWindow.getOptionBoxes();
            for (size_t i = 0; i < boxes.size(); ++i) {
                if (boxes[i].getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    settingsWindow.getOptionBoxes()[settings["UserOptions"]["board_style_index"].get<int>()].setFillColor(sf::Color(150, 150, 250)); 
                    settingsWindow.setSelectedIndex(i); 
                    settingsWindow.getOptionBoxes()[i].setFillColor(sf::Color(100, 100, 200)); 
                    settings["UserOptions"]["board_style"] = settingsWindow.getOptionTexts()[i].getString().toAnsiString();
                    settings["UserOptions"]["board_style_index"] = i;
                    settingsWindow.setSelectedText(settingsWindow.getOptionTexts()[i]);
                    break;
               }
            }
            if (settingsWindow.getApplyChangesButton().getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                std::ofstream file("Settings.json");
                if (!file.is_open()) {
                    throw std::runtime_error("Failed to open settings file: Settings.json");
                }
                file << settings.dump(4);
                file.close();
            }
            if (settingsWindow.getButtonBack().getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                state = GameState::StartScreen;
            }
            else if (settingsWindow.getButtonReset().getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                //resetGame();
                // ovjde moram napraviti da postavke budu resetirane
            }
        }
       else if (state == GameState::ColorSelection) {
                if (buttonWhite.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    startGameWithAI(Figure::white);  
                }
                else if (buttonBlack.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                    startGameWithAI(Figure::black);  
                }
        }
    else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {

        boardWindow.getBoardSquareAt(boardWindow.getSelectedX(), boardWindow.getSelectedY()).setFillColor(boardWindow.getFieldColors()[((boardWindow.getSelectedX() + boardWindow.getSelectedY()) % 2)]);
        boardWindow.setSelectedFigures(0);
    }
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
            boardWindow.setChessPiecesFigure(index, currFigure.figure);
            boardWindow.setChessPiecesColor(index, currFigure.color);
            boardWindow.setChessPieceXY(index, i, j);
            if (currFigure.figure != Figure::Empty) {
                int textureIndex = setTexture(currFigure);
                boardWindow.setChessPieceTexture(index, boardWindow.getPieceTexture(textureIndex));
                boardWindow.setChessPieceDraw(index, 1);
                
            }
            else {
                boardWindow.setChessPieceDraw(index, 0);
            }
            ++index;
        }
    }
    cBoard.bKing_moved = false, cBoard.wKing_moved = false, cBoard.bRook1_moved = false, cBoard.bRook2_moved = false, cBoard.wRook1_moved = false, cBoard.wRook2_moved = false;
    boardWindow.MapPieces();
    boardWindow.FitToHolder();
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
                        win.create(sf::VideoMode(sf::Vector2u(boardWindow.getSX(), boardWindow.getSY())), load_string(CHESS));

                        boardWindow.FitToHolder();
                        boardWindow.MapPieces();
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
        std::optional<sf::Event> event;
        while (auto eventOpt =  Victorywindow.pollEvent()) {
            event = eventOpt;
            if (event->is<sf::Event::Closed>())
                Victorywindow.close();
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(Victorywindow);
                    if (button.getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {

                        win.close();


                        resetGame();

                        state = GameState::StartScreen;
                        win.create(sf::VideoMode(sf::Vector2u(boardWindow.getSX(),  boardWindow.getSY())), load_string(CHESS));

                        boardWindow.FitToHolder();
                        boardWindow.MapPieces();
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
        
        if(state == GameState::StartScreen && (buttonStart.getGlobalBounds().contains(mousePos) || buttonQuit.getGlobalBounds().contains(mousePos)  || buttonSettings.getGlobalBounds().contains(mousePos) || buttonEngine.getGlobalBounds().contains(mousePos)))
        {
            
            win.setMouseCursor(handCursor);
        }
        else 
        {
            win.setMouseCursor(arrowCursor);
        }
        if(state == GameState::Settings && (settingsWindow.getButtonBack().getGlobalBounds().contains(mousePos) || settingsWindow.getButtonReset().getGlobalBounds().contains(mousePos) || settingsWindow.getApplyChangesButton().getGlobalBounds().contains(mousePos)))
        {
            win.setMouseCursor(handCursor);
        }
        else if(state == GameState::Settings) {
            bool overOptionBox = false;
            for (const auto& box : settingsWindow.getOptionBoxes()) {
                if (box.getGlobalBounds().contains(mousePos)) {
                    overOptionBox = true;
                    break;
                }
            }
            bool overOptionText = false;
            for (const auto& text : settingsWindow.getOptionTexts()) {
                if (text.getGlobalBounds().contains(mousePos)) {
                    overOptionText = true;
                    break;
                }
            }
            if (overOptionBox || overOptionText) {
                win.setMouseCursor(handCursor);
            }
        }
        else if (state == GameState::ChessBoard && boardWindow.getSelectedFigures() == 1 && boardWindow.getBoardSquareAt(boardWindow.getSelectedX(), boardWindow.getSelectedY()).getGlobalBounds().contains(mousePos)) {
            win.setMouseCursor(handCursor);
        }
        else if (state == GameState::ChessBoard && boardWindow.getSelectedFigures() == 0) {
            win.setMouseCursor(arrowCursor);
        }
        else if(state == GameState::ColorSelection && (buttonWhite.getGlobalBounds().contains(mousePos) || buttonBlack.getGlobalBounds().contains(mousePos)))
        {
            win.setMouseCursor(handCursor);
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
        win.draw(buttonEngine);
        win.draw(buttonTextEngine);
        win.draw(buttonSettings);
        win.draw(buttonTextSettings);
        win.draw(buttonQuit);
        win.draw(buttonTextQuit);

    }
    else if (state == GameState::ChessBoard) {
        DrawSquares();
        DrawPieces();
    }
    else if (state == GameState::Settings) {
        win.draw(settingsWindow.getBackgroundSprite());
        win.draw(settingsWindow.getApplyChangesButton());
        win.draw(settingsWindow.getButtonTextApplyChanges());
        for (auto& box : settingsWindow.getOptionBoxes()) win.draw(box);
        for (auto& text : settingsWindow.getOptionTexts()) win.draw(text);
        win.draw(settingsWindow.getButtonBack());
        win.draw(settingsWindow.getButtonTextBack());
        win.draw(settingsWindow.getButtonReset());
        win.draw(settingsWindow.getButtonTextReset());
    }
    else if (state == GameState::ColorSelection) {
        showColorSelection();
        win.draw(colorSelectionTitle);
        win.draw(buttonWhite);
        win.draw(buttonTextWhite);
        win.draw(buttonBlack);
        win.draw(buttonTextBlack);
    }
    win.display();
    return true;
}

void chessWin::showColorSelection() {
 

    buttonWhite.setSize(sf::Vector2f(150, 80));
    buttonWhite.setPosition(sf::Vector2f(200, 300));
    buttonWhite.setFillColor(sf::Color::White);
    buttonWhite.setOutlineColor(sf::Color::Black);
    buttonWhite.setOutlineThickness(3);
    
    buttonBlack.setSize(sf::Vector2f(150, 80));
    buttonBlack.setPosition(sf::Vector2f(450, 300));
    buttonBlack.setFillColor(sf::Color::Black);
    buttonBlack.setOutlineColor(sf::Color::White);
    buttonBlack.setOutlineThickness(3);
    
 
    buttonTextWhite = sf::Text(font,load_string(CHOOSE_WHITE), 20);
    buttonTextWhite.setFillColor(sf::Color::Black);
    buttonTextWhite.setPosition(sf::Vector2f(210, 330));

    buttonTextBlack = sf::Text(font, load_string(CHOOSE_BLACK), 20);
    buttonTextBlack.setFillColor(sf::Color::White);
    buttonTextBlack.setPosition(sf::Vector2f(460, 330));
    
    colorSelectionTitle = sf::Text(font, L"Choose Your Color", 30);
    colorSelectionTitle.setFillColor(sf::Color::White);
    colorSelectionTitle.setPosition(sf::Vector2f(250, 200));
}


void chessWin::startGameWithAI(Figure::Colors Color) {
    humanColor = Color;
    playingAgainstAI = true;
    state = GameState::ChessBoard;

    cBoard = chessBoard();
    boardWindow.MapPieces();
  
    if (humanColor == Figure::black) {
                
        std::string currentFEN = cBoard.boardToFEN();       
        std::string bestMove = stockfish.getBestMove(currentFEN);

        if (bestMove.length() >= 4) {
        
            int fromX = bestMove[0] - 'a';  
            int fromY = 7 - (bestMove[1] - '1');  
            int toX = bestMove[2] - 'a';
            int toY = 7 - (bestMove[3] - '1');
            
        
            move m(Point(fromX, fromY), Point(toX, toY));
            std::array<int, 4> replace = {0, 0, 0, 0};
            bool rotation = false;
            bool end = false;
            bool Passant = false;
            Point enPassantPawn;
            
            if (cBoard.playMove(m, replace, end, rotation, enPassantPawn, Passant)) {
                if (rotation) {
                    boardWindow.MapPieces(m);
                    move m2 = move(Point(replace[0], replace[1]), Point(replace[2], replace[3]));
                    boardWindow.MapPieces(m2);
                    cBoard.nextTurn();
                }
                else if (Passant) {
                    boardWindow.MapPieces(m);
                    boardWindow.RemovePieceAt(enPassantPawn);
                    cBoard.nextTurn();
                }
                else {
                    boardWindow.MapPieces(m);
                    if (!end) {
                        cBoard.nextTurn();
                    }
                }
            }
        }
    }
}
bool chessWin::isAITurn() const {
    return playingAgainstAI && (cBoard.turn != humanColor);
}


void chessWin::playAiMove(){
                               
    sf::sleep(sf::milliseconds(200));
                                                                             
    std::string currentFEN = cBoard.boardToFEN(); //Pretvaram ploču u FEN zapis
                                
    std::string bestMove = stockfish.getBestMove(currentFEN); //Stockfish vraća najbolji potez u algebarskoj notaciji

    if (bestMove.length() >= 4) {

        int fromX = bestMove[0] - 'a';
        int fromY = 7 - (bestMove[1] - '1');
        int toX = bestMove[2] - 'a';
        int toY = 7 - (bestMove[3] - '1');

        move aiMove(Point(fromX, fromY), Point(toX, toY));
        std::array<int, 4> aiReplace = {0, 0, 0, 0};
        bool aiRotation = false;
        bool aiEnd = false;
        bool aiPassant = false;
        Point aiEnPassantPawn;
                                  
        if (cBoard.playMove(aiMove, aiReplace, aiEnd, aiRotation, aiEnPassantPawn, aiPassant)) {
            if(aiRotation) {
                boardWindow.MapPieces(aiMove);
                move aiM2 = move(Point(aiReplace[0], aiReplace[1]), Point(aiReplace[2], aiReplace[3]));
                boardWindow.MapPieces(aiM2);
                cBoard.nextTurn();
            } else if (aiPassant) {
                boardWindow.MapPieces(aiMove);
                boardWindow.RemovePieceAt(aiEnPassantPawn);
                cBoard.nextTurn();
            } else {
                boardWindow.MapPieces(aiMove);
                if (aiEnd) {
                DrawSquares();
                DrawPieces();
                win.display();
                drawVictoryWindow(cBoard.turn);
            } else {
                cBoard.nextTurn();
                }
            }
        }   
    }
}

void chessWin::selectFigures(int projX, int projY) {
    
    boardWindow.setSelected(projX, projY);
    boardWindow.getBoardSquareAt(projX, projY).setFillColor(sf::Color::Yellow);
}

void chessWin::deselectFigures(int projX, int projY) {
     boardWindow.getBoardSquareAt(projX, projY).setFillColor(boardWindow.getFieldColors()[((projX + projY) % 2)]);
}
