#include "Game.h"
#include "BattleSystem.h"
#include <iostream>
#include <string>
#include "NPC.h"
#include "Map.h"
#include <vector>
#include <bitset>       // 用于处理位操作
#include <chrono>       // 用于时间/延迟
#include <thread>       // 用于线程休眠
#include <conio.h>    // 为了使用 _kbhit() 和 _getch()
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

void Game::clearScreen() const {
#ifdef _WIN32
    system("cls"); // Windows系统使用 "cls"
#else
    system("clear"); // Linux/macOS系统使用 "clear"
#endif
}

// 帮助函数：设置控制台文本颜色
void SetConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}



// Game构造函数：初始化游戏世界
Game::Game()
    : gameMap(),
    player(gameMap.startRoom),
    mistMonster("迷雾怪物", 200, 25, 10, Item("迷雾核心", "一个强大的能量源", ItemEffect::ATTACK_BUFF, 20)),
    isRunning(true)
{
    gameMap.build();
    player.currentRoom = gameMap.startRoom; // 确保玩家在正确的起始房间
}

//主菜单逻辑
void Game::run() {
    showLogo();
    while (isRunning) {
        showMainMenu();
        std::string choice;
        std::cout << "请输入你的选择: ";
        std::cin >> choice;
        std::cout << "\n----------------------------------------\n";

        if (choice == "1") {
            // 当选择1时，进入并开始探索循环
            clearScreen();

            //TODO:打印函数：可以打印前言


            explorationLoop();
            // 从探索循环退出后，清屏并重新显示Logo，回到主菜单
            clearScreen();
            showLogo();
        }
        else if (choice == "2") {
            showCommands();
        }
        else if (choice == "3") {
            isRunning = false;
            std::cout << "感谢游玩《Mist Monster》。\n";
        }
        else if (choice == "4") {
            challengeMonster();
        }
        else {
            std::cout << "无效的输入，请重新选择。\n";
        }
        std::cout << "----------------------------------------\n\n";
    }
}

void Game::showLogo() const {
    std::cout << "########################################\n";
    std::cout << "#                                      #\n";
    std::cout << "#            Mist Monster              #\n";
    std::cout << "#                                      #\n";
    std::cout << "########################################\n\n";
	std::cout << "欢迎来到《Mist Monster》！\n";
	std::cout << "在这个充满迷雾的世界中，你需要探索发育，击败迷雾怪物并生存下去。\n";
}

void Game::showMainMenu() {
    std::cout << "1. 发育探索\n";
    std::cout << "2. 查看指令\n";
    std::cout << "3. 退出游戏\n";
    std::cout << "4. 挑战迷雾怪物（请确保你的实力足够强大）\n";
}

void Game::explorationLoop() {
    bool isExploring = true;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    // 第一次进入时，先进行一次房间互动检查（例如，如果起始房间有敌人）
    handleRoomInteraction();

    while (isExploring && player.isAlive()) {
        // 1. 刷新界面
        clearScreen();

        // 2. 显示当前环境和状态
        std::cout << "--- 探索模式 ---\n";

        player.currentRoom->look();
        std::cout << std::endl;
        player.showStatus();
        std::cout << "----------------\n";

        // 3. 提示并获取玩家指令
        std::cout << "输入指令 (如 'go north', 'look', 'inv', 'menu' , 'help', 'map', 'status'): \n> ";
        std::string command;
        // 使用 std::cin.ignore() 和 std::getline 来读取带空格的完整指令
       
        std::getline(std::cin, command);

        // 4. 处理玩家指令
        if (command == "menu" || command == "quit" || command == "exit") {
            isExploring = false; // 设置flag以退出探索循环
            std::cout << "正在返回主菜单...\n";
            // 暂停一下让玩家看到信息
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else {
            processExplorationInput(command);
            // 每次行动后，都检查是否触发了房间互动（如进入有怪的房间）
            handleRoomInteraction();
        }
    }

    // 如果玩家死亡，则重置游戏状态
    if (!player.isAlive()) {
        std::cout << "\n你已在探索中倒下...按任意键返回主菜单。\n";
        std::cin.get();
        resetGame();
    }
}

//命令解释
void Game::processExplorationInput(const std::string& input) {
    std::string command;
    std::string argument;
    size_t space_pos = input.find(' ');

    if (space_pos != std::string::npos) {
        command = input.substr(0, space_pos);
        argument = input.substr(space_pos + 1);
    }
    else {
        command = input;
    }

    if (command == "go") {
        player.move(argument);
    }
    else if (command == "map") {
		Map::printMap();
    }
    else if (command == "look") {
        // 'look' 的功能已在循环开始时自动执行
        player.currentRoom->look();
    }
    else if (command == "status") {
        // 'status' 的功能也已自动执行，但同样允许手动查看
        player.showStatus();
    }
    else if (command == "inventory" || command == "inv") {
        player.showInventory();
    }
    else if (command == "use") {
        player.useItem(argument);
    }
    else if (command == "help") {
		showCommands();
    }
    else {
        std::cout << "未知的指令。\n";
    }

}


void Game::handleRoomInteraction() {
    Room* room = player.currentRoom;

    // 遇敌
    if (room->enemy != nullptr) {
        std::cout << "\n你遇到了 " << room->enemy->name << "!\n";
        BattleSystem battle;
        bool playerWon = battle.startBattle(player, *room->enemy);

        if (playerWon) {
            std::cout << "你击败了 " << room->enemy->name << "!\n";
            Item droppedLoot = room->enemy->loot;
            std::cout << "你获得了物品: " << droppedLoot.name << ".\n";
            player.takeItem(droppedLoot);
            delete room->enemy; // 怪物被打败后从房间移除
            room->enemy = nullptr;
        }
        else {
            std::cout << "你被击败了...游戏结束。\n";
            // Game over logic handled in main loop
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }

    // 遇NPC
    if (room->npc != nullptr) {
        room->npc->talk();
        if (!room->npc->hasGivenItem) {
            Item receivedItem = room->npc->giveItem();
            std::cout << "你从 " << room->npc->name << " 那里获得了: " << receivedItem.name << "!\n";
            player.takeItem(receivedItem);
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get();
    }
}

void Game::showCommands() const {

    std::cout << "\n探索模式指令:\n";
    std::cout << "  go [north/south/east/west]: 向指定方向移动。\n";
    std::cout << "  look: 查看地图。\n";
    std::cout << "  status: 查看你的当前状态。\n";
    std::cout << "  inventory (或 inv): 查看你的背包。\n";
    std::cout << "  use [物品名]: 使用背包中的一个物品。\n";
    std::cout << "--------------------\n";
}

void Game::challengeMonster() {
    std::cout << "你鼓起勇气，决定挑战最终的迷雾怪物！\n";
    BattleSystem battle;
    bool playerWon = battle.startBattle(player, mistMonster);

    if (playerWon) {
        std::cout << "\n****************************************\n";
        std::cout << "你成功击败了迷雾怪物！浓雾散去，你找到了出路。\n";
        std::cout << "结局：【逃脱】\n";
        std::cout << "****************************************\n";
        isRunning = false; // 游戏结束
    }
    else {
        std::cout << "\n****************************************\n";
        std::cout << "迷雾吞噬了你，你永远地迷失在了这里。\n";
        std::cout << "结局：【迷失】\n";
        std::cout << "****************************************\n";
        isRunning = false; // 游戏结束
    }
}

void Game::resetGame() {
    std::cout << "\n...回到了初始的界面...\n\n";
    // 创建一个新的Player对象来重置状态
    player = Player(gameMap.startRoom);
}






