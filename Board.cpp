#include <vector>
#include "Board.h"
#include <iostream>
#include <algorithm>
#include <array>

void chessBoard::Pawn(const board& Board,std::vector<move>& moves, const Point& position, Figure::Colors color)
{
    if (Board.arr[position.x][position.y].figure == Figure::Pawn && Board.arr[position.x][position.y].color == color)
    {
        int direction = (color == Figure::white) ? -1 : 1;
        int startRow = (color == Figure::white) ? 6 : 1;
        int opponentColor = (color == Figure::white) ? Figure::black : Figure::white;

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

/*std::vector<move> chessBoard::getLegalMoves(const board& b, Figure::Colors color)
{
    std::vector<move> allMoves;
    std::vector<move> legalMoves;

    // Generiraj sve moguće poteze bez razmišljanja o šahu
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (b.arr[i][j].color != color || b.arr[i][j].figure == Figure::Empty)
                continue;

            Point pos(i, j);
            std::vector<move> pieceMoves;

            switch (b.arr[i][j].figure)
            {
                case Figure::Pawn:   Pawn(b, pieceMoves, pos); break;
                case Figure::Knight: Knight(b, pieceMoves, pos); break;
                case Figure::Bishop: Bishop(b, pieceMoves, pos); break;
                case Figure::Rook:   Rook(b, pieceMoves, pos); break;
                case Figure::Queen:  Queen(b, pieceMoves, pos); break;
                case Figure::King:   King(b, pieceMoves, pos); break;
                default: break;
            }

            allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
        }
    }

    // Sada filtriraj sve poteze koji ostavljaju kralja u šahu
    for (const auto& m : allMoves)
    {
        board temp = b;

        // Napravi potez na kopiji
        temp.arr[m.to.x][m.to.y] = temp.arr[m.from.x][m.from.y];
        temp.arr[m.from.x][m.from.y] = { Figure::Empty, Figure::none };

        // Provjeri je li kralj siguran nakon poteza
        if (!isKingInCheck(temp, color))
        {
            legalMoves.push_back(m);
        }
    }

    return legalMoves;
}*/

std::vector<move> chessBoard::getLegalMoves(const board& b, Figure::Colors color)
{
    std::vector<move> moves;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
                if (b.arr[i][j].figure == Figure::Pawn && b.arr[i][j].color == color)
                {
                    Pawn(b,moves, Point(i, j), color);
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
    std::vector<move> opponentMoves = getLegalMoves(b,(color == Figure::white) ? Figure::black : Figure::white);
    return std::any_of(opponentMoves.begin(), opponentMoves.end(), [&](const move& m) { return m.to == kingPos; });
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

/*bool chessBoard::playMove(move req, std::array<int,4>& replace,bool& end,bool& rotation,Point& enPassantPawn,bool& Passant)
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
}*/
bool chessBoard::playMove(move req, std::array<int,4>& replace, bool& end, bool& rotation, Point& enPassantPawn, bool& Passant)
{
    std::vector<move> legalMoves = getLegalMoves(chessBoard, turn);

    bool isLegal = false;
    for (const auto& m : legalMoves) {
        if (m == req) {
            isLegal = true;
            break;
        }
    }

    if (!isLegal) return false;

    
    board tempBoard = chessBoard;
    
    bool isPawnDoubleMove = (tempBoard.arr[req.from.x][req.from.y].figure == Figure::Pawn &&
                              std::abs(req.to.y - req.from.y) == 2);

    
    bool isCastling = false;
    int rookFromX = -1, rookToX = -1;
    if (tempBoard.arr[req.from.x][req.from.y].figure == Figure::King &&
        std::abs(req.to.x - req.from.x) == 2) {
        isCastling = true;
        if (req.to.x > req.from.x) {
            rookFromX = 7;
            rookToX = 5;
        } else {
            rookFromX = 0;
            rookToX = 3;
        }
    }

    
    Passant = false;
    if (tempBoard.arr[req.from.x][req.from.y].figure == Figure::Pawn && req.to == enPassantTarget) {
        int direction = (turn == Figure::white) ? 1 : -1;
        tempBoard.arr[req.to.x][req.to.y] = tempBoard.arr[req.from.x][req.from.y];
        tempBoard.arr[req.from.x][req.from.y] = { Figure::Empty, Figure::none };
        tempBoard.arr[req.to.x][req.to.y + direction] = { Figure::Empty, Figure::none };
        enPassantPawn = Point(req.to.x, req.to.y + direction);
        Passant = true;
    } else {
    
        tempBoard.arr[req.to.x][req.to.y] = tempBoard.arr[req.from.x][req.from.y];
        tempBoard.arr[req.from.x][req.from.y] = { Figure::Empty, Figure::none };

        if (isCastling) {
            tempBoard.arr[rookToX][req.from.y] = tempBoard.arr[rookFromX][req.from.y];
            tempBoard.arr[rookFromX][req.from.y] = { Figure::Empty, Figure::none };
            replace = { rookFromX, req.from.y, rookToX, req.from.y };
            rotation = true;
        }
    }

    
    if (isKingInCheck(tempBoard, turn)) {
        return false;
    }

    
    chessBoard = tempBoard;

    
    if (isPawnDoubleMove) {
        enPassantPossible = true;
        enPassantTarget = Point(req.from.x, req.from.y + ((turn == Figure::white) ? -1 : 1));
    } else {
        enPassantPossible = false;
    }

    
    Figure::Colors opponent = (turn == Figure::white) ? Figure::black : Figure::white;
    if (isCheckmate(chessBoard, opponent)) {
        end = true;
    }

    
    if (chessBoard.arr[req.to.x][req.to.y].figure == Figure::King) {
        if (turn == Figure::white) wKing_moved = true;
        else bKing_moved = true;
    } else if (chessBoard.arr[req.to.x][req.to.y].figure == Figure::Rook) {
        if (turn == Figure::white) {
            if (req.from.x == 0 && req.from.y == 7) wRook1_moved = true;
            else if (req.from.x == 7 && req.from.y == 7) wRook2_moved = true;
        } else {
            if (req.from.x == 0 && req.from.y == 0) bRook1_moved = true;
            else if (req.from.x == 7 && req.from.y == 0) bRook2_moved = true;
        }
    }

    
    history.push_back(chessBoard);

    return true;
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
// Add this method to your chessBoard class
std::string chessBoard::boardToFEN() const {
    std::string fen = "";
    
    // 1. Piece placement (from rank 8 to rank 1, file a to h)
    for (int rank = 0; rank < 8; ++rank) {
        int emptyCount = 0;
        
        for (int file = 0; file < 8; ++file) {
            Figure piece = chessBoard.arr[file][rank];
            
            if (piece.figure == Figure::Empty) {
                emptyCount++;
            } else {
                // Add empty squares count if any
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                
                // Add piece character
                char pieceChar = getPieceChar(piece);
                fen += pieceChar;
            }
        }
        
        // Add remaining empty squares at end of rank
        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        
        // Add rank separator (except for last rank)
        if (rank < 7) {
            fen += "/";
        }
    }
    
    // 2. Active color
    fen += " ";
    fen += (turn == Figure::white) ? "w" : "b";
    
    // 3. Castling availability
    fen += " ";
    std::string castling = "";
    if (!wKing_moved && !wRook2_moved) castling += "K";  // White kingside
    if (!wKing_moved && !wRook1_moved) castling += "Q";  // White queenside
    if (!bKing_moved && !bRook2_moved) castling += "k";  // Black kingside
    if (!bKing_moved && !bRook1_moved) castling += "q";  // Black queenside
    
    if (castling.empty()) {
        fen += "-";
    } else {
        fen += castling;
    }
    
    // 4. En passant target square
    fen += " ";
    if (enPassantPossible) {
        // Convert Point to algebraic notation (e.g., Point(4,2) -> "e3")
        char file = 'a' + enPassantTarget.x;
        char rank = '1' + (7 - enPassantTarget.y);  // Flip rank for FEN
        fen += file;
        fen += rank;
    } else {
        fen += "-";
    }
    
    // 5. Halfmove clock (moves since last capture or pawn move)
    // You might need to track this separately
    fen += " 0";
    
    // 6. Fullmove number (increments after Black's move)
    // You might need to track this separately
    fen += " 1";
    
    return fen;
}

// Helper method to convert piece to FEN character
char chessBoard::getPieceChar(const Figure& piece) const {
    char baseChar;
    
    switch (piece.figure) {
        case Figure::Pawn:   baseChar = 'p'; break;
        case Figure::Rook:   baseChar = 'r'; break;
        case Figure::Knight: baseChar = 'n'; break;
        case Figure::Bishop: baseChar = 'b'; break;
        case Figure::Queen:  baseChar = 'q'; break;
        case Figure::King:   baseChar = 'k'; break;
        default:             return ' ';  // Should not happen
    }
    
 return (piece.color == Figure::white) ? std::toupper(baseChar) : baseChar;
}