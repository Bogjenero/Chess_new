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

using json = nlohmann::json;
sf::Texture emptyTexture; 

void from_json(const json& j, RGBColor& color) {
    j.at("r").get_to(color.r);
    j.at("g").get_to(color.g);
    j.at("b").get_to(color.b);
}



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
    { RESET, L"Reset Game" },
    { ENGINE, L"Against computer" },
    { COLOR_SELECTION, L"Color Selection" },
    { CHOOSE_WHITE, L"Play as White" },
    { CHOOSE_BLACK, L"Play as Black" },
    { BLACK_WHITE, L"Black and White " },
    { LIGHT_WOOD, L"Light Wood" },
    { DARK_WOOD, L"Dark Wood" },
    { BLUE_GRAY, L"Blue Gray" },
    { GREEN_MARBLE, L"Green Marble" },
    { SLATE_DARK, L"Slate Dark" },
    { APPLY, L"Apply Changes" }

};
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
std::wstring load_string(Strings uID) {

    static const std::wstring emptyString = L"";
    auto it = stringMap.find(uID);
    return (it != stringMap.end()) ? it->second : emptyString;

}

StockFish::StockFish() {

    #ifdef _WIN32
        stockFish = popen("stockfish.exe", "r");
    #elif __linux__
        if (pipe(toStockfish) == -1 || pipe(fromStockfish) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // child process - Stockfish

            
            dup2(toStockfish[0], STDIN_FILENO);
            dup2(fromStockfish[1], STDOUT_FILENO);

            // Zatvori nepotrebne pipe-ove
            close(toStockfish[1]);
            close(fromStockfish[0]);

            execl("./StockFish/stockfish/stockfish-ubuntu-x86-64-avx2", "stockfish-ubuntu", nullptr);
            perror("execl failed");
            exit(EXIT_FAILURE);
        } else {
            // parent process
            close(toStockfish[0]);   // zatvori čitanje pipea za slanje Stockfishu
            close(fromStockfish[1]); // zatvori pisanje pipea za čitanje od Stockfisha              
            
            sendCommand("uci");

            std::string response;
            do {
                response = getResponse();
            } while (response.find("uciok") == std::string::npos);
                   
            sendCommand("isready");
            do {
                response = getResponse();
            } while (response.find("readyok") == std::string::npos);
        }
    #endif
}


void StockFish::sendCommand(const std::string& command) {
    #ifdef _WIN32
        fprintf(stockFish, "%s\n", command.c_str());
        fflush(stockFish);
    #elif __linux__
        write(toStockfish[1], command.c_str(), command.size());
        write(toStockfish[1], "\n", 1); // dodaj novi red
    #endif
}
std::string StockFish::getResponse() {
    std::string response;
    char buffer[256];
    ssize_t bytesRead;
    
    #ifdef _WIN32
        while (fgets(buffer, sizeof(buffer), stockFish) != nullptr) {
            response += buffer;
        }
    #elif __linux__
      fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(fromStockfish[0], &readfds);

        timeout.tv_sec = 1;         // maksimalno čekaj 1 sekundu
        timeout.tv_usec = 0;

        int ready = select(fromStockfish[0] + 1, &readfds, nullptr, nullptr, &timeout);
        if (ready > 0 && FD_ISSET(fromStockfish[0], &readfds)) {
            bytesRead = read(fromStockfish[0], buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
            }
        }
    #endif
    
    return response;
}


std::string StockFish::getBestMove(const std::string& fenPosition) {

    sendCommand("position fen " + fenPosition);
    
    sendCommand("go depth 10");  

    std::string response;
    do {
        response = getResponse();
    } while (response.find("bestmove") == std::string::npos);
    

    size_t pos = response.find("bestmove ") + 9;
    return response.substr(pos, 4);  
}



chessWin::chessWin(): buttonTextStart( font, load_string(START), 30 ), buttonTextQuit(font,load_string(QUIT),30),buttonTextSettings(font,load_string(SETTINGS),30), 
 backgroundSprite(backgroundTexture), buttonTextEngine(font, load_string(ENGINE), 30), 
 buttonTextWhite(font, load_string(CHOOSE_WHITE), 30), buttonTextBlack(font, load_string(CHOOSE_BLACK), 30),
 colorSelectionTitle(font, load_string(COLOR_SELECTION), 30) {}


chessPiece::chessPiece() : Sprite(emptyTexture) {}



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
 boardWin::boardWin() : boardSprite(emptyTexture), boardTextture(emptyTexture) {

 }   


settingsWin::settingsWin() : buttonTextBack(font, load_string(BACK), 30), 
buttonBack(sf::Vector2f(200, 60)), buttonTextReset(font, load_string(RESET), 30), buttonReset(sf::Vector2f(200, 60)),
backgroundSprite(backgroundTexture), selectBox(sf::Vector2f(200, 60)), selectBoxText(font, L"Select Board", 30), selectedText(font, L"Selected: ", 30),
applyChangesButton(sf::Vector2f(200, 60)), buttonTextApplyChanges(font, load_string(APPLY), 30)
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
        
    int selectedIndex = 0;

    applyChangesButton.setPosition(sf::Vector2f((width - 200) / 2.f, 300));
    applyChangesButton.setFillColor(sf::Color::Green);
    applyChangesButton.setOutlineThickness(2);

    sf::Vector2f buttonPos = applyChangesButton.getPosition();
    sf::Vector2f buttonSize = applyChangesButton.getSize();
    buttonTextApplyChanges.setPosition(sf::Vector2f(buttonPos.x + buttonSize.x / 2.0f, buttonPos.y + buttonSize.y / 2.0f));
    buttonTextApplyChanges.setFillColor(sf::Color::White);
    

    sf::FloatRect textBoundsApply = buttonTextApplyChanges.getLocalBounds();

    buttonTextApplyChanges.setOrigin(sf::Vector2f(textBoundsApply.position.x + textBoundsApply.size.x / 2.0f, textBoundsApply.position.y + textBoundsApply.size.y / 2.0f));
    //buttonTextReset.setOrigin(sf::Vector2f(textBoundsReset.position.x + textBoundsReset.size.x / 2.0f, textBoundsReset.position.y + textBoundsReset.size.y / 2.0f));


    for (size_t i = 0; i < boardOptions.size(); ++i) {
        sf::RectangleShape box(sf::Vector2f(200, 30));
        if (i == selectedIndex) {
            box.setFillColor(sf::Color(100, 100, 200)); 
        } else {
            box.setFillColor(sf::Color(150, 150, 250));
        }
        box.setPosition(sf::Vector2f((width - 200) / 2.f, 80 + i * 30)); 
        optionBoxes.push_back(box);

        sf::Text text(font, load_string(boardOptions[i]), 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f((width - 195) / 2.f, 85 + i * 30));
        optionTexts.push_back(text);
    }    
    
    


    buttonReset.setPosition(sf::Vector2f((width - 200) / 2.f, 400));
    buttonReset.setFillColor(sf::Color::Green);
    buttonReset.setOutlineThickness(2);

    
    buttonBack.setPosition(sf::Vector2f((width - 200) / 2.f, 500));
    buttonBack.setFillColor(sf::Color::Blue);
    buttonBack.setOutlineThickness(2);

    
    
    sf::FloatRect textBounds = buttonTextBack.getLocalBounds();
    buttonTextBack.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    
    buttonPos = buttonBack.getPosition();
    buttonSize = buttonBack.getSize();
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
  backgroundSprite(backgroundTexture), buttonTextEngine(font, load_string(ENGINE), 25), 
  buttonTextWhite(font, load_string(WHITE_WINS), 25), buttonTextBlack(font, load_string(BLACK_WINS), 25),
colorSelectionTitle(font, load_string(CHESS), 30)
{

    
    std::ifstream file("Settings.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open settings file: Settings.json");
    } 
    nlohmann::json settings;
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
                        settingsWindow.setSelectedIndex(i); 
                        std::cout << "Selected option: " << i << std::endl;
                        break;
                    }
                }

                
                const auto& texts = settingsWindow.getOptionTexts();
                for (size_t i = 0; i < texts.size(); ++i) {
                    if (texts[i].getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y)) ) {
                        settingsWindow.setSelectedText(texts[i]);
                        std::cout << "Selected option text: " << texts[i].getString().toAnsiString() << std::endl;
                        break;
                    }
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
