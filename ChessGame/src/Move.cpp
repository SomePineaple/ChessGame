#include "Move.h"

#include <iostream>

void Move::SetMove(int startx, int starty, int destx, int desty, std::string board[8][8]) {
	this->startx = startx;
	this->starty = starty;
	this->destx = destx;
	this->desty = desty;

	hash = starty * 1000 + startx * 100 + desty * 10 + destx;

	pieceMoved = board[startx][starty];
	pieceCaptured = board[destx][desty];
}

void Move::MakeMove(std::string board[8][8]) {
	if (board[startx][starty] == "--")
		return;
	board[startx][starty] = "--";
	board[destx][desty] = pieceMoved;
}

void Move::UndoMove(std::string board[8][8]) {
	board[startx][starty] = pieceMoved;
	board[destx][desty] = pieceCaptured;
}

bool Move::operator ==(const Move &move2) {
	return hash == move2.hash;
}

/*-----------------------------------------------------------------------------------------------------------------------*/

// Castle moves

/*-----------------------------------------------------------------------------------------------------------------------*/

void CastleMove::SetMove(int startx, int starty, int destx, int desty, std::string board[8][8]) {
	this->startx = startx;
	this->starty = starty;
	this->destx = destx;
	this->desty = desty;

	hash = starty * 1000 + startx * 100 + desty * 10 + destx;

	pieceMoved = board[startx][starty];
	pieceCaptured = board[destx][desty];

	isCastleMove = true;
}

void CastleMove::MakeMove(std::string board[8][8]) {
	board[startx][starty] = "--";
	if (color) {
		if (desty == 0) {
			board[7][0] = "--";
			board[7][1] = "wK";
			board[7][2] = "wR";
		} else {
			board[7][7] = "--";
			board[7][6] = "wK";
			board[7][5] = "wR";
		}
	} else {
		if (desty == 0) {
			board[0][0] = "--";
			board[0][1] = "bK";
			board[0][2] = "bR";
		} else {
			board[0][7] = "--";
			board[0][6] = "bK";
			board[0][5] = "bR";
		}
	}
}

void CastleMove::UndoMove(std::string board[8][8]) {
	std::cout << "Here\n";
	if (color) {
		if (desty == 0) {
			board[7][0] = "wR";
			board[7][1] = "--";
			board[7][2] = "--";
			board[7][3] = "--";
			board[7][4] = "wK";
		} else {
			board[7][7] = "wR";
			board[7][6] = "--";
			board[7][5] = "--";
			board[7][4] = "wK";
		}
	} else {
		if (desty == 0) {
			board[0][0] = "wR";
			board[0][1] = "--";
			board[0][2] = "--";
			board[0][3] = "--";
			board[0][4] = "wK";
		} else {
			board[0][7] = "wR";
			board[0][6] = "--";
			board[0][5] = "--";
			board[0][4] = "wK";
		}
	}
}
