#ifndef MOVE_H_
#define MOVE_H_

#include <string>

class Move{
public:
    Move(int startx, int starty, int destx, int desty, std::string board[8][8]);
    std::string pieceMoved;
    std::string pieceCaptured;
    void MakeMove(std::string board[8][8]);

    bool operator ==(const Move &move2);

    int hash;
private:
    int startx;
    int starty;
    int destx;
    int desty;
    const char white = 'w';
    const char black = 'b';
};

#endif /* MOVE_H_ */
