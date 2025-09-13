#pragma once
#include <vector>
#include <iostream>

class TicTacToeGame {
private:
    std::vector<char> board;
    void printBoard() const;
    bool isMoveValid(int move) const;
    char checkWinner();
    void computerMove();

public:
    TicTacToeGame();
    bool playGame(); // 主游戏循环，如果玩家赢了返回 true
};
