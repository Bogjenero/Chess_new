#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window.hpp>
#include <cstdio>
#include <memory>
#include <iostream>



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
