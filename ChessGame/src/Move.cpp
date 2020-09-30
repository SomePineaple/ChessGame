#include "Move.h"

#include <iostream>

Move::Move(int startx, int starty, int destx, int desty, std::string board[8][8]){
	this->startx = startx;
	this->starty = starty;
	this->destx = destx;
	this->desty = desty;

	hash = startx * 1000 + starty * 100 + destx * 10 + desty;
}

void Move::MakeMove(std::string board[8][8]) {
	if (board[starty][startx] == "--")
		return;
	std::string pieceMoved = board[starty][startx];
	board[starty][startx] = "--";
	board[desty][destx] = pieceMoved;
	std::cout << pieceMoved << " from " << startx << ", " << starty << " to " << destx << ", " << desty << std::endl;
}

bool Move::operator ==(const Move &move2) {
	return hash == move2.hash;
}
