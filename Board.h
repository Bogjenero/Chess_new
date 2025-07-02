#pragma once
#include <vector>


struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point operator+(const Point& other) const {
        return { x + other.x,y + other.y };
    };
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool isValid() {
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}
};

struct move
{
    Point from;
    Point to;
    move(const Point& oldPoint, const Point& newPoint)
        : from(oldPoint.x,oldPoint.y), to(newPoint.x,newPoint.y) {}
    move() : from(Point()), to(Point()) {} 
    bool operator==(const move& other) const {
        return (from == other.from) && (to == other.to);
    }
};

struct Figure {
    enum Figures {
        Empty ,Pawn, Rook, Knight, Bishop, King, Queen
    };
    enum Colors {
        none = -1,white, black
    };
    Figures figure;
    Colors color;   
};

struct board
{
        Figure arr[8][8] = {
            {{Figure::Rook, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Rook, Figure::white}},
            {{Figure::Knight, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Knight, Figure::white}},
            {{Figure::Bishop, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Bishop, Figure::white}},
            {{Figure::Queen, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Queen, Figure::white}},
            {{Figure::King, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::King, Figure::white}},
            {{Figure::Bishop, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Bishop, Figure::white}},
            {{Figure::Knight, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Knight, Figure::white}},
            {{Figure::Rook, Figure::black}, {Figure::Pawn, Figure::black}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Empty, Figure::none}, {Figure::Pawn, Figure::white}, {Figure::Rook, Figure::white}}
        };        
};

class chessBoard {
private: 
    std::vector<board> history;
    void Pawn(const board& Board, std::vector<move>& moves, const Point& position); // potezi za bijelog pijuna
    void King(const board& Board, std::vector<move>& moves, const Point& position); // potezi za kralja
    void Rook(const board& Board, std::vector<move>& moves, const Point& position); // potezi za topa
    void Queen(const board& Board, std::vector<move>& moves, const Point& position); // potezi za topa
    void Bishop(const board& Board, std::vector<move>& moves, const Point& position); // potezi za topa
    void Knight(const board& Board, std::vector<move>& moves, const Point& position); // potezi za topa
    std::vector<move> getLegalMoves(const board& b, Figure::Colors color); // dohvaća sve legalne poteze za određenu boju
    bool isKingInCheck(const board& b, Figure::Colors color); // provjerava je li kralj u šahu
    bool isSquareUnderAttack(const Point& p); // provjerava je li polje pod napadom
    bool isCheckmate(const board& b, Figure::Colors turn);
    bool enPassantPossible = false;
    Point enPassantTarget;
    Point enPassantPawn;
public:
    bool wKing_moved = false, bKing_moved = false, wRook1_moved = false, wRook2_moved = false, bRook1_moved = false, bRook2_moved = false;
    Figure::Colors turn = Figure::white;
    board chessBoard;
    bool playMove(move req,std::array<int,4>& replace,bool& end,bool& rotation,Point& enPassantPawn,bool& Passant); // izvršava potez
    void nextTurn(); // prebacuje na sljedeći potez
};    
