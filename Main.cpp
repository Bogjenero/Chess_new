#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "resource.h"
#include "Iscrtavanje.h"



using namespace sf;
int main()
{

	const std::string defaultTheme[12] = { 
									"./images/Pieces/Default/wp.png",
								"./images/Pieces/Default/wr.png",
								"./images/Pieces/Default/wn.png",
								"./images/Pieces/Default/wb.png",
								"./images/Pieces/Default/wk.png",
								"./images/Pieces/Default/wq.png",
								"./images/Pieces/Default/bp.png",
								"./images/Pieces/Default/br.png",
								"./images/Pieces/Default/bn.png",
								"./images/Pieces/Default/bb.png",
								"./images/Pieces/Default/bk.png",
								"./images/Pieces/Default/bq.png" };
	chessWin temp;
	chessWin window(800, 800, temp.load_string(CHESS), defaultTheme);

	while (window.Update())
	{
	
	}
	return 0;
}

