#include "Engine.h"

#include <iostream>

using namespace std;

Engine::Engine() {
}

Engine::~Engine() {
}

// Makes a move, moving a piece from the start position (startx, starty) to the end position (destx, desty)
void Engine::MakeMove(int startx, int starty, int destx, int desty){

	// Contains all the valid moves
    list<Move> possibleMoves = GetValidMoves();

    // All this stuff is to check if the list possibleMoves contains the current move we want to do
    bool validMove = false;

    Move move;
	move.SetMove(starty, startx, desty, destx);

    for (Move possibleMove : possibleMoves) {
    	if (possibleMove == move) {
    		validMove = true;
    	}
    }

    // If it wasn't a valid move
    if (!validMove) {
    	cout << "Not a valid move\n";
    	return;
    }

    // If it was a valid move, we can now make that move
    move.MakeMove(board);

    // Pawn promotion
    if (move.pieceMoved == "wP" && desty == 0) {
    	cout << "Setting pawn as queen\n";
    	SetPiece(destx, desty, "wQ");
    } else if (move.pieceMoved == "bP" && desty == 7) {
    	cout << "Setting pawn as queen\n";
    	SetPiece(destx, desty, "bQ");
    }

    completedMoves.push_back(move);

    whiteMove = !whiteMove;
}

list<Move> Engine::GetValidMoves(){
	return GetAllMoves();
}

list<Move> Engine::GetAllMoves() {
	list<Move> movelist;
	// Loop through all the pieces on the board
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			char turn = GetPiece(c, r)[0];
			// Make sure that its not an empty square
			if (turn == '-')
				continue;
			// Only add the moves for the current players turn to the move list
			if ((turn == 'w' && whiteMove) || (turn == 'b' && !whiteMove)) {
				char piece = GetPiece(c, r)[1];
				// Check what piece it is and add the moves for that piece to the move list
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

void Engine::GetPawnMoves(int r, int c, list<Move> &movelist) {
	if (whiteMove) {
		if (GetPiece(c, r - 1) == "--") {
			Move move;
			move.SetMove(r, c, r - 1, c);
			movelist.push_back(move);
			if (r == 6 && GetPiece(c, r - 2) == "--") {
				Move move2;
				move2.SetMove(r, c, r - 2, c);
				movelist.push_back(move2);
			}
		}

		if (GetPiece(c - 1, r - 1)[0] == BLACK) {
			Move move3;
			move3.SetMove(r, c, r - 1, c - 1);
			movelist.push_back(move3);
		}

		if (GetPiece(c + 1, r - 1)[0] == BLACK) {
			Move move;
			move.SetMove(r, c, r - 1, c + 1);
			movelist.push_back(move);
		}

	} else{
		if (GetPiece(c, r + 1) == "--" && r + 1 <= 7) {
			Move move;
			move.SetMove(r, c, r + 1, c);
			movelist.push_back(move);
			if (r == 1 && GetPiece(c, r + 2) == "--" && r + 2 <= 7) {
				Move move2;
				move2.SetMove(r, c, r + 2, c);
				movelist.push_back(move2);
			}
		}

		if (GetPiece(c - 1, r + 1)[0] == WHITE) {
			Move move;
			move.SetMove(r, c, r + 1, c - 1);
			movelist.push_back(move);
		}

		if (GetPiece(c + 1, r + 1)[0] == WHITE) {
			Move move;
			move.SetMove(r, c, r + 1, c + 1);
			movelist.push_back(move);
		}
	}
}

void Engine::GetRookMoves(int r, int c, list<Move> &movelist) {
	list<list<int>> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
	char enemyColor = whiteMove ? BLACK : WHITE;

	for (list<int> d : directions) {
		for (int i = 1; i < 8; i++) {
			int endRow = r + d.front() * i;
			int endCol = c + d.back() * i;
			if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
				string endPiece = GetPiece(endCol, endRow);
				if (endPiece == "--") {
					Move move;
					move.SetMove(r, c, endRow, endCol);
					movelist.push_back(move);
				} else if (endPiece[0] == enemyColor) {
					Move move;
					move.SetMove(r, c, endRow, endCol);
					movelist.push_back(move);
					break;
				} else {
					break;
				}
			} else {
				break;
			}
		}
	}
}

void Engine::UndoMove() {
	Move move = completedMoves.back();

	SetPiece(move.destx, move.desty, move.pieceCaptured);
	SetPiece(move.startx, move.starty, move.pieceMoved);

	completedMoves.pop_back();

	whiteMove = !whiteMove;
}

void Engine::SetPiece(int x, int y, std::string piece) {
	board[y][x] = piece;
}

string Engine::GetPiece(int x, int y) {
	return board[y][x];
}
