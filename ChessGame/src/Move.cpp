#include "Move.h"

#include <iostream>

void Move::SetMove(int startx, int starty, int destx, int desty) {
	this->startx = startx;
	this->starty = starty;
	this->destx = destx;
	this->desty = desty;

	hash = starty * 1000 + startx * 100 + desty * 10 + destx;
}

void Move::MakeMove(std::string board[8][8]) {
	if (board[startx][starty] == "--")
		return;
	pieceMoved = board[startx][starty];
	pieceCaptured = board[destx][desty];
	board[startx][starty] = "--";
	board[destx][desty] = pieceMoved;
	std::cout << pieceMoved << " from " << startx << ", " << starty << " to " << destx << ", " << desty << std::endl;
}

bool Move::operator ==(const Move &move2) {
	return hash == move2.hash;
}
