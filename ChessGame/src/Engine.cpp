#include "Engine.h"

#include <initializer_list>
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
    bool useCastleMove = false;

    Move move;
	move.SetMove(starty, startx, desty, destx, board);

	CastleMove castleMove;

    for (Move possibleMove : possibleMoves) {
    	if (possibleMove == move) {
    		validMove = true;

    		if (possibleMove.isCastleMove) {
    			cout << "Was a castle move\n";
    			useCastleMove = true;
    			castleMove.SetMove(starty, startx, desty, destx, board);
    			castleMove.color = whiteMove;
    		}
    	}
    }

    // If it wasn't a valid move
    if (!validMove) {
    	cout << "Not a valid move\n";
    	return;
    }

    // If it was a valid move, we can now make that move
    if (!useCastleMove)
    	move.MakeMove(board);
    else
    	castleMove.MakeMove(board);

    // Pawn promotion
    if (move.pieceMoved == "wP" && desty == 0) {
    	cout << "Setting pawn as queen\n";
    	SetPiece(destx, desty, "wQ");
    } else if (move.pieceMoved == "bP" && desty == 7) {
    	cout << "Setting pawn as queen\n";
    	SetPiece(destx, desty, "bQ");
    }

    // Updating kings location
    else if (move.pieceMoved == "wK") {
    	whiteKingLoc = {desty, destx};
    	hasWhiteKingMoved = true;
    } else if (move.pieceMoved == "bK") {
    	blackKingLoc = {desty, destx};
    	hasBlackKingMoved = true;
    }

    // Making sure the rooks havn't moved, needed for castling
    else if (move.pieceMoved == "wR" && move.starty == 0) {
    	hasWhiteLeftRookMoved = true;
    } else if (move.pieceMoved == "wR" && move.starty == 7) {
    	hasWhiteRightRookMoved = true;
    } else if (move.pieceMoved == "bR" && move.starty == 0) {
    	hasBlackLeftRookMoved = true;
    } else if (move.pieceMoved == "bR" && move.starty == 7) {
    	hasBlackRightRookMoved = true;
    }

    cout << move.pieceMoved << " to " << destx << " " << desty << endl;

    if (!useCastleMove)
    	completedMoves.push_back(move);
    else
    	completedMoves.push_back(castleMove);

    whiteMove = !whiteMove;

    // Just to update the check and checkmate variables
    GetValidMoves();
}

// Will check for moves that can't be completed because of check and stuff like that
list<Move> Engine::GetValidMoves(){
	list<Move> allMoves = GetAllMoves();
	list<Move> badMoves;

	// Set checkmate to true, so if it doesn't get set back to false, we know that there is a checkmate
	checkMate = true;

	// For all the moves, make them, if the player is in check, add that move to the list of bad moves
	for (Move m : allMoves) {
		bool check;
		m.MakeMove(board);
		completedMoves.push_back(m);
		check = InCheck();
		whiteMove = !whiteMove;
		UndoMove();
		if (check) {
			cout << "Check with move " << m.starty << m.startx << m.desty << m.destx << endl;
			badMoves.push_back(m);
		}
		else
			checkMate = false;
	}

	// Get rid of all the bad moves
	for (Move m : badMoves)
		allMoves.remove(m);

	return allMoves;
}

bool Engine::InCheck() {
	if (whiteMove) {
		return IsKingAttacked(true);
	} else {
		return IsKingAttacked(false);
	}
}

// Checks if a certain players king is in check
bool Engine::IsKingAttacked(bool white) {
	whiteMove = !whiteMove;

	list<Move> movelist = GetAllMoves();

	whiteMove = !whiteMove;

	for (Move m : movelist) {
		if (m.pieceCaptured == "wK" && white) {
			return true;
		} if (m.pieceCaptured == "bK" && !white) {
			return true;
		}
	}

	return false;
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
					GetCastleMoves(r, c, movelist);
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
			move.SetMove(r, c, r - 1, c, board);
			movelist.push_back(move);

			// A pawn can move two spaces on its first turn, which would be when its on row 6
			if (r == 6 && GetPiece(c, r - 2) == "--") {
				Move move2;
				move2.SetMove(r, c, r - 2, c, board);
				movelist.push_back(move2);
			}
		}

		// Doing diagonal moves
		if (GetPiece(c - 1, r - 1)[0] == BLACK && c - 1 >= 0) {
			Move move3;
			move3.SetMove(r, c, r - 1, c - 1, board);
			movelist.push_back(move3);
		}

		if (GetPiece(c + 1, r - 1)[0] == BLACK && c + 1 <= 7) {
			Move move;
			move.SetMove(r, c, r - 1, c + 1, board);
			movelist.push_back(move);
		}

	} else{
		// This is for black pawns, same thing as before, but going in the other direction
		if (GetPiece(c, r + 1) == "--" && r + 1 <= 7) {
			Move move;
			move.SetMove(r, c, r + 1, c, board);
			movelist.push_back(move);
			if (r == 1 && GetPiece(c, r + 2) == "--" && r + 2 <= 7) {
				Move move2;
				move2.SetMove(r, c, r + 2, c, board);
				movelist.push_back(move2);
			}
		}

		if (GetPiece(c - 1, r + 1)[0] == WHITE && c - 1 >= 0) {
			Move move;
			move.SetMove(r, c, r + 1, c - 1, board);
			movelist.push_back(move);
		}

		if (GetPiece(c + 1, r + 1)[0] == WHITE && c + 1 <= 7) {
			Move move;
			move.SetMove(r, c, r + 1, c + 1, board);
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
					move.SetMove(r, c, endRow, endCol, board);
					movelist.push_back(move);
				} else if (endPiece[0] == enemyColor) {
					// If there is an enemy, add that move to the move list, and then break,
					// this break means don't go farther because you can't go through pieces
					Move move;
					move.SetMove(r, c, endRow, endCol, board);
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
				move.SetMove(r, c, endRow, endCol, board);
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
					move.SetMove(r, c, endRow, endCol, board);
					movelist.push_back(move);
				} else if (endPiece[0] == enemyColor) {
					Move move;
					move.SetMove(r, c, endRow, endCol, board);
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
					move.SetMove(r, c, endRow, endCol, board);
					movelist.push_back(move);
				} else if (endPiece[0] == enemyColor) {
					Move move;
					move.SetMove(r, c, endRow, endCol, board);
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
				move.SetMove(r, c, endRow, endCol, board);
				movelist.push_back(move);
			}
		}
	}
}

void Engine::GetCastleMoves(int r, int c, list<Move> &movelist) {
	if (whiteMove && hasWhiteKingMoved)
		return;
	else if (!whiteMove && hasBlackKingMoved)
		return;

	if (GetPiece(3, r) == "--" && GetPiece(2, r) == "--" && GetPiece(1, r) == "--" && (whiteMove ? !hasWhiteLeftRookMoved : !hasBlackLeftRookMoved)) {
		CastleMove move;
		move.SetMove(r, c, r, 0, board);
		move.color = whiteMove;
		movelist.push_back(move);
	}

	if (GetPiece(5, r) == "--" && GetPiece(6, r) == "--" && (whiteMove ? !hasWhiteRightRookMoved : !hasBlackRightRookMoved)) {
		CastleMove move;
		move.SetMove(r, c, r, 7, board);
		move.color = whiteMove;
		movelist.push_back(move);
	}
}

void Engine::UndoMove() {
	if (completedMoves.size() == 0)
		return;

	Move move = completedMoves.back();

	move.UndoMove(board);

	if (move.pieceMoved == "wK")
		whiteKingLoc = {move.starty, move.startx};

	else if (move.pieceMoved == "bK")
		blackKingLoc = {move.starty, move.startx};

	completedMoves.pop_back();

	whiteMove = !whiteMove;
}

void Engine::SetPiece(int x, int y, std::string piece) {
	board[y][x] = piece;
}

string Engine::GetPiece(int x, int y) {
	return board[y][x];
}
