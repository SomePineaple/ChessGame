#include "Engine.h"

#include <iostream>

using namespace std;

Engine::Engine() {
}

Engine::~Engine() {
}

void Engine::MakeMove(int startx, int starty, int destx, int desty){
    string pieceMoved = board[starty][startx];
    list<list<int>> possibleMoves = GetAllMoves();
    bool validMove = false;
    list<int> movelist = {starty, startx, desty, destx};
    for (list<int> re : possibleMoves) {
    	if (re == movelist) {
    		validMove = true;
    	}
    }
    if (!validMove) {
    	cout << "Not a valid move\n";
    	return;
    }
    Move *move = new Move(startx, starty, destx, desty, board);
    move->MakeMove(board);
    delete move;
    whiteMove = !whiteMove;
}

list<list<int>> Engine::GetAllMoves() {
	list<list<int>> movelist;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			char turn = board[r][c][0];
			if (turn == '-')
				continue;
			if ((turn == 'w' && whiteMove) || (turn == 'b' && !whiteMove)) {
				char piece = board[r][c][1];
				switch (piece) {
				case 'P':
					GetPawnMoves(r, c, movelist);
					break;
				case 'R':
					GetRookMoves(r, c, movelist);
					break;
				default:
					break;
				}
			}
		}
	}
	return movelist;
}

void Engine::GetPawnMoves(int r, int c, list<list<int>> &movelist) {
	if (whiteMove) {
		if (board[r - 1][c] == "--") {
			list<int> move = {r, c, r + 1, c};
			movelist.push_back(move);
			if (r == 6 && board[r-2][c] == "--") {
				list<int> move2 = {r, c, r - 2, c};
				movelist.push_back(move2);
			}
		}
		if (board[r - 1][c - 1][0] == 'b') {
			list<int> move = {r, c, r - 1, c - 1};
			movelist.push_back(move);
		}
		if (board[r - 1][c + 1][0] == 'b') {
			list<int> move = {r, c, r - 1, c + 1};
			movelist.push_back(move);
		}
	} else{
		if (board[r + 1][c] == "--") {
			list<int> move = {r, c, r + 1, c};
			movelist.push_back(move);
			if (r == 1 && board[r + 2][c] == "--") {
				list<int> move2 = {r, c, r + 2, c};
				movelist.push_back(move2);
			}
		}
		if (board[r + 1][c - 1][0] == 'b') {
			list<int> move = {r, c, r + 1, c + 1};
			movelist.push_back(move);
		}
		if (board[r + 1][c + 1][0] == 'b') {
			list<int> move = {r, c, r + 1, c + 1};
			movelist.push_back(move);
		}
	}
}

void Engine::GetRookMoves(int r, int c, std::list<std::list<int>> &movelist) {

}
