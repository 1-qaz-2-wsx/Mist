#pragma once
#include "Player.h"
#include "Map.h"
#include "Enemy.h"

class Game {
private:
    Player player;
    Map gameMap;
    Enemy mistMonster;
    bool isRunning;

    void showLogo() const;
    void showMainMenu();
    void processPlayerInput(const std::string& input);
    void explore();
    void showCommands() const;
    void challengeMonster();
    void handleRoomInteraction();
    void resetGame();


public:
    Game();
    void run();
};