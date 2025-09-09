#include "Game.h"
#include <iostream>
#include <Windows.h> // <--- 1. 包含这个新的头文件
int main() {
    // 是一个Windows特有的指令，它会把当前控制台的输出编码设置为UTF-8，完美解决乱码问题。
    SetConsoleOutputCP(CP_UTF8);
    Game game;
    // 错误1修正：调用 initialize 而不是 init
    if (game.initialize("data")) { // 假设你的json文件在 "data" 文件夹下
        // 错误2修正：调用 run 而不是 gameLoop
        game.run();
    }
    else {
        std::cout << "无法启动游戏，请检查数据文件。" << std::endl;
    }

    return 0;
}