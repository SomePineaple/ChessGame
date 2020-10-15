#ifndef MOVE_H_
#define MOVE_H_

#include <string>

class Move{
public:
    std::string pieceMoved;
    std::string pieceCaptured;

    void SetMove(int startx, int starty, int destx, int desty, std::string board[8][8]);

    bool isCastleMove = false;

    void MakeMove(std::string board[8][8]);

    void UndoMove(std::string board[8][8]);

    bool operator ==(const Move &move2);

    int startx;
	int starty;
	int destx;
	int desty;

    int hash;
private:
    const char white = 'w';
    const char black = 'b';
};

class CastleMove: public Move{
public:
	void SetMove(int startx, int starty, int dest, int desty, std::string board[8][8]);

	void MakeMove(std::string board[8][8]);
	void UndoMove(std::string board[8][8]);
	bool color;
};

#endif /* MOVE_H_ */
