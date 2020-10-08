#ifndef ENGINE_H
#define ENGINE_H

#include <list>
#include <string>

#include "Move.h"

class Engine {
public:
	bool whiteMove = true;

	const char WHITE = 'w';
	const char BLACK = 'b';

    Engine();
    ~Engine();
    void MakeMove(int startx, int starty, int destx, int desty);

    void UndoMove();

    void SetPiece(int x, int y, std::string piece);

    std::string GetPiece(int x, int y);

private:
    std::list<Move> GetValidMoves();
    std::list<Move> GetAllMoves();

    bool InCheck();

    void GetPawnMoves(int r, int c, std::list<Move> &movelist);
    void GetRookMoves(int r, int c, std::list<Move> &movelist);
    void GetBishopMoves(int r, int c, std::list<Move> &movelist);
    void GetKnightMoves(int r, int c, std::list<Move> &movelist);
    void GetQueenMoves(int r, int c, std::list<Move> &movelist);
    void GetKingMoves(int r, int c, std::list<Move> &movelist);

    std::list<Move> completedMoves;

    std::string board[8][8] = {
    {"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
    {"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
    {"--", "--", "--", "--", "--", "--", "--", "--"},
    {"--", "--", "--", "--", "--", "--", "--", "--"},
    {"--", "--", "--", "--", "--", "--", "--", "--"},
    {"--", "--", "--", "--", "--", "--", "--", "--"},
    {"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
    {"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}};
};

#endif /* ENGINE_H_ */
