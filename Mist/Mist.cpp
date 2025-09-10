#include "Game.h"
#include <ctime>
#include <windows.h>
int main() {
    SetConsoleOutputCP(65001);

    // 将控制台的输入编码设置为 UTF-8
    SetConsoleCP(65001);
    // 设置随机数种子，用于战斗等
    srand(static_cast<unsigned int>(time(0)));

    Game game;
    game.run();

    return 0;
}