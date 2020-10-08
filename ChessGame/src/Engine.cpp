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

// Will check for moves that can't be completed because of check and stuff like that
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
				case 'N':
					GetKnightMoves(r, c, movelist);
					break;
				case 'B':
					GetBishopMoves(r, c, movelist);
					break;
				case 'Q':
					GetQueenMoves(r, c, movelist);
					break;
				case 'K':
					GetKingMoves(r, c, movelist);
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
		// If the piece in front of the pawn is empty
		if (GetPiece(c, r - 1) == "--") {
			Move move;
			move.SetMove(r, c, r - 1, c);
			movelist.push_back(move);

			// A pawn can move two spaces on its first turn, which would be when its on row 6
			if (r == 6 && GetPiece(c, r - 2) == "--") {
				Move move2;
				move2.SetMove(r, c, r - 2, c);
				movelist.push_back(move2);
			}
		}

		// Doing diagonal moves
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
		// This is for black pawns, same thing as before, but going in the other direction
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
	// All the one square moves a rook can make, going in a direction 1 at a time
	list<list<int>> directions = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
	char enemyColor = whiteMove ? BLACK : WHITE;

	for (list<int> d : directions) {
		for (int i = 1; i < 8; i++) {
			// Get the destination
			int endRow = r + d.front() * i;
			int endCol = c + d.back() * i;
			// Check to make sure we are still on the board
			if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
				string endPiece = GetPiece(endCol, endRow);
				if (endPiece == "--") {
					// If there is an empty space, add it to the move list
					Move move;
					move.SetMove(r, c, endRow, endCol);
					movelist.push_back(move);
				} else if (endPiece[0] == enemyColor) {
					// If there is an enemy, add that move to the move list, and then break,
					// this break means don't go farther because you can't go through pieces
					Move move;
					move.SetMove(r, c, endRow, endCol);
					movelist.push_back(move);
					break;
				} else {
					// If there is a friendly piece, don't add to the move list, because you can't capture friendly pieces
					break;
				}
			} else {
				break;
			}
		}
	}
}

void Engine::GetKnightMoves(int r, int c, list<Move> &movelist) {
	// All the possible moves a knight can make
	list<list<int>> moves = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
	char allyColor = whiteMove ? WHITE : BLACK;
	for (list<int> m : moves) {
		int endRow = r + m.front();
		int endCol = c + m.back();
		if ( 0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
			string endPiece = GetPiece(endCol, endRow);
			if (endPiece[0] != allyColor) {
				Move move;
				move.SetMove(r, c, endRow, endCol);
				movelist.push_back(move);
			}
		}
	}
}

void Engine::GetBishopMoves(int r, int c, list<Move> &movelist) {
	// All this is the same as the rook moves, but in different directions
	list<list<int>> directions = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
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

void Engine::GetQueenMoves(int r, int c, std::list<Move> &movelist) {
	// This is all the same as the rook and bishop moves, except with more directions
	list<list<int>> directions = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}};
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

void Engine::GetKingMoves(int r, int c, list<Move> &movelist) {
	// Actually just the same thing as the knight, but with different directions
	list<list<int>> moves = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {1, 0}, {0, 1}};
	char allyColor = whiteMove ? WHITE : BLACK;

	for (list<int> m : moves) {
		int endRow = r + m.front();
		int endCol = c + m.back();
		if ( 0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8) {
			string endPiece = GetPiece(endCol, endRow);
			if (endPiece[0] != allyColor) {
				Move move;
				move.SetMove(r, c, endRow, endCol);
				movelist.push_back(move);
			}
		}
	}
}

void Engine::UndoMove() {
	if (completedMoves.size() == 0)
		return;

	Move move = completedMoves.back();

	SetPiece(move.desty, move.destx, move.pieceCaptured);
	SetPiece(move.starty, move.startx, move.pieceMoved);

	completedMoves.pop_back();

	whiteMove = !whiteMove;

	if (!whiteMove)
		UndoMove();
}

void Engine::SetPiece(int x, int y, std::string piece) {
	board[y][x] = piece;
}

string Engine::GetPiece(int x, int y) {
	return board[y][x];
}
