#pragma once
#include "Player.h"
#include "Map.h"
#include "Enemy.h"

#include <iostream>  
#include <sstream>  
#include <string> 
#include <conio.h>
#include <bitset>
#include <iostream>  
#include <windows.h>  
#include<vector>
#include <assert.h>
#include <map>

#include "SlowPrint.h"



class Game {
private:
    Player player;
    Map gameMap;
    Enemy mistMonster;
    bool isRunning;

    void showLogo() const;
    void showMainMenu();
    void explorationLoop();
    void showCommands() const;
    void challengeMonster();
    void handleRoomInteraction();
    void resetGame();
	//辅助函数，探索模式输入处理
    void processExplorationInput(const std::string& input);
    //清屏函数
    void clearScreen() const;
    //读存档
    void saveGame() const;
    bool loadGame(); // 返回 bool 表示是否加载成功


public:
    Game();
    void run();
};







