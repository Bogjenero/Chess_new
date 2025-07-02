#include <vector>
#include "Board.h"
#include <iostream>
#include <algorithm>
#include <array>

void chessBoard::Pawn(const board& Board,std::vector<move>& moves, const Point& position)
{
    if (Board.arr[position.x][position.y].figure == Figure::Pawn && Board.arr[position.x][position.y].color == turn)
    {
        int direction = (turn == Figure::white) ? -1 : 1; 
        int startRow = (turn == Figure::white) ? 6 : 1;   
        int opponentColor = (turn == Figure::white) ? Figure::black : Figure::white;

        if (position.y + direction >= 0 && position.y + direction < 8 && Board.arr[position.x][position.y + direction].figure == Figure::Empty)
        {
            moves.push_back(move(Point(position.x,position.y),Point(position.x, position.y + direction)));

            if (position.y == startRow && Board.arr[position.x][position.y + 2 * direction].figure == Figure::Empty)
            {
                moves.push_back(move(Point(position.x, position.y),Point(position.x, position.y + 2 * direction)));
            }
        }
        if (position.x > 0 && position.y + direction >= 0 && position.y + direction < 8)
        {
            if (Board.arr[position.x - 1][position.y + direction].color == opponentColor && Board.arr[position.x - 1][position.y + direction].figure != Figure::Empty)
            {
                moves.push_back(move(Point(position.x, position.y),Point(position.x - 1, position.y + direction)));
            }
        }
        if (position.x < 7 && position.y + direction >= 0 && position.y + direction < 8)
        {
            if (Board.arr[position.x + 1][position.y + direction].color == opponentColor && Board.arr[position.x + 1][position.y + direction].figure != Figure::Empty)
            {
                moves.push_back(move(Point(position.x, position.y),Point(position.x + 1, position.y + direction)));
            }
        }
        if (enPassantPossible) {
            if (position.y == (turn == Figure::white ? 3 : 4)) {
                if (position.x > 0 && enPassantTarget == Point(position.x - 1, position.y + direction)) {
                moves.push_back(move(Point(position.x, position.y), Point(position.x - 1, position.y + direction)));
                }
                if (position.x < 7 && enPassantTarget == Point(position.x + 1, position.y + direction)) {
                moves.push_back(move(Point(position.x, position.y), Point(position.x + 1, position.y + direction)));
                }
            }
        }
    }
}
void chessBoard::King(const board& Board, std::vector<move>& moves, const Point& position)
{
    std::array<Point, 8> directions = {
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1),                Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    };
    for (const auto& dir : directions)
    {
        Point newPos = position + dir;
        if (newPos.isValid())
        {
            if (Board.arr[newPos.x][newPos.y].color != Board.arr[position.x][position.y].color || Board.arr[newPos.x][newPos.y].figure == Figure::Empty)
            {
                moves.push_back(move(position, newPos));
            }
        }
    }

    if (Board.arr[position.x][position.y].color == Figure::white && !wKing_moved)
    {
        if (position.x + 2 < 8 && Board.arr[position.x + 2][position.y].figure == Figure::Empty && Board.arr[position.x + 1][position.y].figure == Figure::Empty)
        {
            moves.push_back(move(position, Point(position.x + 2, position.y)));
        }
        if (position.x - 2 >= 0 && Board.arr[position.x - 2][position.y].figure == Figure::Empty && Board.arr[position.x - 1][position.y].figure == Figure::Empty)
        {
            moves.push_back(move(position, Point(position.x - 2, position.y)));
        }
    }
    else if (Board.arr[position.x][position.y].color == Figure::black && !bKing_moved)
    {
        if (position.x + 2 < 8 && Board.arr[position.x + 2][position.y].figure == Figure::Empty && Board.arr[position.x + 1][position.y].figure == Figure::Empty)
        {
            moves.push_back(move(position, Point(position.x + 2, position.y)));
        }
        if (Board.arr[position.x - 1][position.y].figure == Figure::Empty && Board.arr[position.x - 2][position.y].figure == Figure::Empty)
        {
            moves.push_back(move(position, Point(position.x -2, position.y)));
        }
    }

}

void chessBoard::Knight(const board& Board, std::vector<move>& moves, const Point& position)
{

    if (Board.arr[position.x][position.y].figure != Figure::Knight)
        return;
    
    std::array<Point, 8> directions = {
    Point(-2, 1), Point(-1, 2), Point(1, 2), Point(2, 1),
    Point(2, -1), Point(1, -2), Point(-1, -2), Point(-2, -1)};

    for (const auto& direction : directions)
    {
        Point newPosition = Point(position.x,position.y) + direction;  

        if (newPosition.isValid())  
        {
            if (Board.arr[newPosition.x][newPosition.y].color != Board.arr[position.x][position.y].color || Board.arr[newPosition.x][newPosition.y].figure == Figure::Empty)
            {
                moves.push_back(move(Point(position.x,position.y), newPosition));  
            }
        }
    }

}
void chessBoard::Rook(const board& Board, std::vector<move>& moves, const Point& position)
{
    if(Board.arr[position.x][position.y].figure != Figure::Rook)
	{
		return;
	}
    std::array<Point, 4> directions = {
        Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1)  
    };

    for (const auto& direction : directions)
    {
        Point current = position;

        while (true)
        {
            current = current + direction; 

            if (!current.isValid()) 
            {
                break;
            }

            if (Board.arr[current.x][current.y].figure == Figure::Empty)
            {
                moves.push_back(move(position, current));  
            }
            else if (Board.arr[current.x][current.y].color != Board.arr[position.x][position.y].color)
            {
                moves.push_back(move(position, current)); 
                break;
            }
            else
            {
                break;  
            }
        }
    }
}
void chessBoard::Bishop(const board& Board, std::vector<move>& moves, const Point& position)
{
    if (Board.arr[position.x][position.y].figure != Figure::Bishop)
        return;

    std::array<Point, 4> directions = {
        Point(1, 1), Point(1, -1),
        Point(-1, 1), Point(-1, -1)
    };

    for (const auto& dir : directions)
    {
        Point newPos = position;

        while (true)
        {
            newPos = newPos + dir;

            if (!newPos.isValid())
            {
                break;
            }

            if (Board.arr[newPos.x][newPos.y].figure == Figure::Empty)
            {
                moves.push_back(move(position, newPos));
            }
            else if (Board.arr[newPos.x][newPos.y].color != Board.arr[position.x][position.y].color)
            {
                moves.push_back(move(position, newPos));
                break;
            }
            else
            {
                break;
            }
        }
    }
}


void chessBoard::Queen(const board& Board, std::vector<move>& moves, const Point& position)
{
    if (Board.arr[position.x][position.y].figure != Figure::Queen)
        return;

    std::array<Point, 8> directions = {
        Point(1, 1), Point(1, -1), Point(-1, 1), Point(-1, -1),
        Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) 
    };

    for (const auto& dir : directions)
    {
        Point newPos = position;

        while (true)
        {
            newPos = newPos + dir;

            if (!newPos.isValid())
            {
                break;
            }

            if (Board.arr[newPos.x][newPos.y].figure == Figure::Empty)
            {
                moves.push_back(move(position, newPos));
            }
            else if (Board.arr[newPos.x][newPos.y].color != Board.arr[position.x][position.y].color)
            {
                moves.push_back(move(position, newPos));
                break;
            }
            else
            {
                break;
            }
        }
    }
}



std::vector<move> chessBoard::getLegalMoves(const board& b, Figure::Colors color)
{
    std::vector<move> moves;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
                if (b.arr[i][j].figure == Figure::Pawn && b.arr[i][j].color == color)
                {
                    Pawn(b,moves, Point(i, j));
                }
                else if (b.arr[i][j].figure == Figure::King && b.arr[i][j].color == color)
                {
                    King(b,moves, Point(i, j));
                }
                if (b.arr[i][j].figure == Figure::Knight && b.arr[i][j].color == color)
                {
                    Knight(b,moves, Point(i, j));
                }
                else if (b.arr[i][j].figure == Figure::Rook && b.arr[i][j].color == color)
                {
                    Rook(b,moves, Point(i, j));
                }
                else if (b.arr[i][j].figure == Figure::Bishop && b.arr[i][j].color == color)
                {
                    Bishop(b,moves, Point(i, j));
                }
                else if (b.arr[i][j].figure == Figure::Queen && b.arr[i][j].color == color)
                {
                    Queen(b,moves, Point(i, j));
                }
        }
    }
    return moves;
}


bool chessBoard::isKingInCheck(const board& b,Figure::Colors color)
{
    Point kingPos;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (b.arr[i][j].figure == Figure::King && b.arr[i][j].color == color)
            {
                kingPos = Point(i, j);
                break;
            }
        }
    }
    std::vector<move> opponentMoves;
    opponentMoves = getLegalMoves(b, (color == Figure::white) ? Figure::black : Figure::white);

    return std::any_of(opponentMoves.begin(), opponentMoves.end(), [&](const move& m) {
        return m.to == kingPos;
        });
}

bool chessBoard::isCheckmate(const board& b, Figure::Colors color) {
    
    if (!isKingInCheck(b, color)) {
        return false; 
    }

    std::vector<move> allPossibleMoves = getLegalMoves(b, color);

    return std::none_of(allPossibleMoves.begin(), allPossibleMoves.end(), [&](const move& m) {
        board tempBoard = b;
        tempBoard.arr[m.to.x][m.to.y] = tempBoard.arr[m.from.x][m.from.y];
        tempBoard.arr[m.from.x][m.from.y] = { Figure::Empty, Figure::none };       
        return !isKingInCheck(tempBoard, color);
        });
}


bool chessBoard::isSquareUnderAttack(const Point& p) {
    if (isKingInCheck(chessBoard, turn)) {
        return false;
    }
    std::vector<move> moves;
    moves = getLegalMoves(chessBoard, turn == Figure::white ? Figure::black : Figure::white);
    return !std::any_of(moves.begin(), moves.end(), [&p](const move& m) {
        return m.to == p;
        });
}

bool chessBoard::playMove(move req, std::array<int,4>& replace,bool& end,bool& rotation,Point& enPassantPawn,bool& Passant)
{   
    std::vector<move> moves = getLegalMoves(chessBoard, turn);
    move temp;
    for (int i = 0; i < moves.size(); ++i)
    {
        temp = moves[i];
        if (req==moves[i])
        {
            
           if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::Pawn && req.to == enPassantTarget)
            {
                int direction = (turn == Figure::white) ? 1 : -1;

                chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                chessBoard.arr[req.from.x][req.from.y] = { Figure::Empty, Figure::none };
                chessBoard.arr[req.to.x][req.to.y + direction] = { Figure::Empty, Figure::none };
                enPassantPawn = Point(req.to.x, req.to.y + direction);
                Passant = true;
            }
            else if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::King && chessBoard.arr[req.from.x][req.from.y].color == Figure::white && req.from.x + 2 == temp.to.x && !wKing_moved && !wRook2_moved)
            {
                if (isSquareUnderAttack(Point(req.from.x + 2, req.from.y)) && isSquareUnderAttack(Point(req.from.x + 1, req.from.y)))
                {
                    chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                    chessBoard.arr[req.from.x][req.from.y].figure = Figure::Empty;
                    chessBoard.arr[5][7] = chessBoard.arr[7][7];
                    chessBoard.arr[7][7].figure = Figure::Empty;
                    wKing_moved = true;
                    wRook2_moved = true;
                    replace[0] = 7, replace[1] = 7, replace[2] = 5, replace[3] = 7;               
                    rotation = true;
                }
                else {
                    return false;
                }
            }
            else if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::King && chessBoard.arr[req.from.x][req.from.y].color == Figure::white && req.from.x - 2 == temp.to.x && !wKing_moved && !wRook1_moved)
            {
                if(isSquareUnderAttack(Point(req.from.x-3, req.from.y)) && isSquareUnderAttack(Point(req.from.x-2, req.from.y)) && isSquareUnderAttack(Point(req.from.x-1, req.from.y)))
                {
                    chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                    chessBoard.arr[req.from.x][req.from.y].figure = Figure::Empty;
                    chessBoard.arr[3][7] = chessBoard.arr[0][7];
                    chessBoard.arr[0][7].figure = Figure::Empty;
                    wKing_moved = true;
                    wRook1_moved = true;
                    replace[0] = 0, replace[1] = 7, replace[2] = 3, replace[3] = 7;
                    rotation = true;
                }
                else {
                    return false;   
                }
            }
            else if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::King && chessBoard.arr[req.from.x][req.from.y].color == Figure::black && req.from.x + 2 == temp.to.x && !bKing_moved && !bRook1_moved)
            {
                if (isSquareUnderAttack(Point(req.from.x+1, req.from.y)) && isSquareUnderAttack(Point(req.from.x + 2, req.from.y)))
                {
                    chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                    chessBoard.arr[req.from.x][req.from.y].figure = Figure::Empty;
                    chessBoard.arr[5][0] = chessBoard.arr[7][0];
                    chessBoard.arr[7][0].figure = Figure::Empty;
                    bKing_moved = true;
                    bRook1_moved = true;
                    replace[0] = 7, replace[1] = 0, replace[2] = 5, replace[3] = 0;
                    rotation = true;
                }
                else {
                    return false;
                }
            }
            else if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::King && chessBoard.arr[req.from.x][req.from.y].color == Figure::black && req.from.x - 2 == temp.to.x && !bKing_moved && !wRook2_moved)
            {
                if (isSquareUnderAttack(Point(req.from.x - 3, req.from.y)) && isSquareUnderAttack(Point(req.from.x - 2, req.from.y)) && isSquareUnderAttack(Point(req.from.x-3, req.from.y)))
                {
                    chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                    chessBoard.arr[req.from.x][req.from.y].figure = Figure::Empty;                   
                    chessBoard.arr[3][0] = chessBoard.arr[0][0];
                    chessBoard.arr[0][0].figure = Figure::Empty;
                    bKing_moved = true;
                    bRook2_moved = true;
                    replace[0] = 0, replace[1] = 0, replace[2] = 3, replace[3] = 0;
                    rotation = true;
                }
                else {
                    return false;
                }
            }
            else {
               if (chessBoard.arr[req.from.x][req.from.y].figure == Figure::Pawn && abs(req.to.y - req.from.y) == 2) {
                   enPassantPossible = true;
                   enPassantTarget = Point(req.from.x, req.from.y + (turn == Figure::white ? -1 : 1));
               }
               else {
                   enPassantPossible = false;

               }
                chessBoard.arr[req.to.x][req.to.y] = chessBoard.arr[req.from.x][req.from.y];
                chessBoard.arr[req.from.x][req.from.y].figure = Figure::Empty;
            }
            bool check = false;
            Figure::Colors opponentColor = (turn == Figure::white) ? Figure::black : Figure::white;
            if (isKingInCheck(chessBoard, turn))
            {
                chessBoard = history[history.size() - 1];
                check = true;
                break;
            }
            if (isCheckmate(chessBoard, opponentColor))
            {
                end = true;
                return true;
            }
            if (!check)
            {
                if (chessBoard.arr[req.to.x][req.to.y].figure == Figure::King)
                {
                    (turn == Figure::white) ? wKing_moved = true : (turn == Figure::black) ? bKing_moved = true : false;
                }
                if (chessBoard.arr[req.to.x][req.to.y].figure == Figure::Rook)
				{
					if (turn == Figure::white)
					{
						if (req.from.x == 0 && req.from.y == 7)
						{
							wRook1_moved = true;
						}
						else if (req.from.x == 7 && req.from.y == 7)
						{
							wRook2_moved = true;
						}
					}
					else
					{
						if (req.from.x == 0 && req.from.y == 0)
						{
							bRook1_moved = true;
						}
						else if (req.from.x == 7 && req.from.y == 7)
						{
							bRook2_moved = true;
						}
					}
				}
                history.push_back(chessBoard);
                return true;
            }
        }
    }
    return false;
}
void chessBoard::nextTurn()
{
    if(turn == Figure::white)
	{
		turn = Figure::black;
	}
	else
	{
		turn = Figure::white;
	}
    if (!enPassantPossible) {
        enPassantTarget = Point(-1, -1);
    }
}