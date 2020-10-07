#ifndef MOVE_H_
#define MOVE_H_

#include <string>

class Move{
public:
    std::string pieceMoved;
    std::string pieceCaptured;

    void SetMove(int startx, int starty, int destx, int desty);

    void MakeMove(std::string board[8][8]);

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

#endif /* MOVE_H_ */
