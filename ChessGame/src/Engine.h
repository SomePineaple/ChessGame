#ifndef ENGINE_H
#define ENGINE_H

#include <list>
#include <string>

#include "Move.h"

class Engine {
public:
    Engine();
    ~Engine();
    void MakeMove(int startx, int starty, int destx, int desty);

    std::string board[8][8] = {
	{"bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR"},
	{"bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP"},
	{"--", "--", "--", "--", "--", "--", "--", "--"},
	{"--", "--", "--", "--", "--", "--", "--", "--"},
	{"--", "--", "--", "--", "--", "--", "--", "--"},
	{"--", "--", "--", "--", "--", "--", "--", "--"},
	{"wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP"},
	{"wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"}};
private:
    bool whiteMove = true;
    std::list<std::list<int>> GetAllMoves();
    void GetPawnMoves(int r, int c, std::list<std::list<int>> &movelist);
    void GetRookMoves(int r, int c, std::list<std::list<int>> &movelist);
};

#endif /* ENGINE_H_ */
