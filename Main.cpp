#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Iscrtavanje.h"
#include "StockFish.h"
#include "Board.h"
#include <nlohmann/json.hpp>

using namespace sf;
int main()
{

	const std::string defaultTheme[12] = { 
									//"./images/Pieces/Default/wp.png",
								//"./images/Pieces/Default/wr.png",
								
								//"./images/Pieces/Default/wn.png",
								
								
								"./images/Pieces/Modern/PawnW.png",
								"./images/Pieces/Modern/RookW.png",
								"./images/Pieces/Modern/KnightW.png",
								"./images/Pieces/Modern/BishopW.png",
								"./images/Pieces/Modern/KingW.png",
								"./images/Pieces/Modern/QueenW.png",
								
								
								"./images/Pawn.png",
								"./images/Rook.png",
								"./images/Knight.png",
								"./images/Bishop.png",
								"./images/King.png",
								"./images/Queen.png",
								};


	//chessWin temp;
	//chessWin window(800, 800, load_string(CHESS), defaultTheme);
	chessWin window;
	StockFish stockfish;

	while (window.Update()) {
		
	}
	
	
	

	
	return 0;
}

