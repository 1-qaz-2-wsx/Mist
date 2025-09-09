#include "Game.h"
#include <iostream>

int main() {
    Game game;
    if (!game.init("map.json")) {  // 目前 Map::load 里是手工填的
        std::cerr << "初始化失败\n";
        return 1;
    }
    game.gameLoop();
    return 0;
}
