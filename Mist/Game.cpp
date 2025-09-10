#include "Game.h"
#include "BattleSystem.h"
#include <iostream>
#include <string>
#include "NPC.h"

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

void Game::run() {
    showLogo();
    while (isRunning) {
        showMainMenu();
        std::string choice;
        std::cout << "请输入你的选择: ";
        std::cin >> choice;
        std::cout << "\n----------------------------------------\n";

        if (choice == "1") {
            explore();
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
}

void Game::showMainMenu() {
    std::cout << "你现在该做什么？\n";
    std::cout << "1. 发育探索\n";
    std::cout << "2. 查看指令\n";
    std::cout << "3. 退出游戏\n";
    std::cout << "4. 挑战迷雾怪物\n";
}

void Game::explore() {
    player.currentRoom->look();
    handleRoomInteraction();

    if (!player.isAlive()) {
        resetGame();
        return;
    }

    std::cout << "\n接下来要做什么？ (输入 'help' 查看探索指令): ";
    std::string command;
    std::cin.ignore(); // 清除之前的换行符
    std::getline(std::cin, command);
    processPlayerInput(command);
}

void Game::processPlayerInput(const std::string& input) {
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
    else if (command == "look") {
        player.currentRoom->look();
    }
    else if (command == "status") {
        player.showStatus();
    }
    else if (command == "inventory" || command == "inv") {
        player.showInventory();
    }
    else if (command == "use") {
        player.useItem(argument);
    }
    else if (command == "help") {
        std::cout << "探索指令: go [方向], look, status, inventory, use [物品名]\n";
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
    }

    // 遇NPC
    if (room->npc != nullptr) {
        room->npc->talk();
        if (!room->npc->hasGivenItem) {
            Item receivedItem = room->npc->giveItem();
            std::cout << "你从 " << room->npc->name << " 那里获得了: " << receivedItem.name << "!\n";
            player.takeItem(receivedItem);
        }
    }
}

void Game::showCommands() const {
    std::cout << "--- 游戏指令列表 ---\n";
    std::cout << "主菜单指令:\n";
    std::cout << "  1: 进入探索模式，你可以在地图上移动和互动。\n";
    std::cout << "  2: 显示此帮助列表。\n";
    std::cout << "  3: 退出游戏。\n";
    std::cout << "  4: 直接挑战最终Boss迷雾怪物。\n";
    std::cout << "\n探索模式指令:\n";
    std::cout << "  go [north/south/east/west]: 向指定方向移动。\n";
    std::cout << "  look: 查看当前房间的描述。\n";
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