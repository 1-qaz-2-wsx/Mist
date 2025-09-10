#include "Game.h"
#include <iostream>
#include <Windows.h> // <--- 1. 包含这个新的头文件
int main() {
    // 是一个Windows特有的指令，它会把当前控制台的输出编码设置为UTF-8，解决乱码问题。
    SetConsoleOutputCP(CP_UTF8);
    // 设置控制台输入编码为 UTF-8
    SetConsoleCP(CP_UTF8);

    Game game;
    if (game.initialize("data")) { // json文件在 "data" 文件夹下
        game.run();
    }
    else {
        std::cout << "无法启动游戏，请检查数据文件。" << std::endl;
    }
    return 0;
}