// 文件路径: Mist/TicTacToeGame.cpp

#include "TicTacToeGame.h"
#include <iostream>
#include <vector>
#include <limits>
#include <random>

TicTacToeGame::TicTacToeGame() : board(9, ' ') {} // 初始化 3x3 的棋盘为空格

void TicTacToeGame::printBoard() const {
    std::cout << "\n";
    std::cout << " " << board[0] << " | " << board[1] << " | " << board[2] << "   (1, 2, 3)\n";
    std::cout << "---|---|---\n";
    std::cout << " " << board[3] << " | " << board[4] << " | " << board[5] << "   (4, 5, 6)\n";
    std::cout << "---|---|---\n";
    std::cout << " " << board[6] << " | " << board[7] << " | " << board[8] << "   (7, 8, 9)\n";
    std::cout << "\n";
}

bool TicTacToeGame::isMoveValid(int move) const {
    return (move >= 0 && move < 9 && board[move] == ' ');
}

char TicTacToeGame::checkWinner() {
    // 检查行
    for (int i = 0; i < 3; ++i) {
        if (board[i * 3] != ' ' && board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2]) return board[i * 3];
    }
    // 检查列
    for (int i = 0; i < 3; ++i) {
        if (board[i] != ' ' && board[i] == board[i + 3] && board[i + 3] == board[i + 6]) return board[i];
    }
    // 检查对角线
    if (board[0] != ' ' && board[0] == board[4] && board[4] == board[8]) return board[0];
    if (board[2] != ' ' && board[2] == board[4] && board[4] == board[6]) return board[2];
    // 检查是否平局
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') return ' '; // 游戏继续
    }
    return 'D'; // Draw (平局)
}

void TicTacToeGame::computerMove() {
    std::vector<int> possibleMoves;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            possibleMoves.push_back(i);
        }
    }
    if (!possibleMoves.empty()) {
        int move = possibleMoves[rand() % possibleMoves.size()];
        board[move] = 'O';
        std::cout << "柯洁将棋子放在了 " << move + 1 << " 号位。\n";
    }
}

bool TicTacToeGame::playGame() {
    std::cout << "你接受了挑战！你执 'X'，柯洁执 'O'。\n";
    char winner = ' ';

    while (winner == ' ') {
        printBoard();
        int move;
        std::cout << "请选择你要落子的位置 (1-9): ";
        std::cin >> move;

        while (std::cin.fail() || !isMoveValid(move - 1)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效的输入或位置已被占用，请重新选择: ";
            std::cin >> move;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        board[move - 1] = 'X';
        winner = checkWinner();

        if (winner == ' ') {
            computerMove();
            winner = checkWinner();
        }
    }

    printBoard();
    if (winner == 'X') {
        std::cout << "恭喜你，你赢了！\n";
        return true;
    }
    else if (winner == 'O') {
        std::cout << "很遗憾，你输了。\n";
        return false;
    }
    else {
        std::cout << "平局！再接再厉。\n";
        return false;
    }
}