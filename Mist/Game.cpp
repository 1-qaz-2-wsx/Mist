#include "Game.h"
#include "BattleSystem.h"
#include <iostream>
#include <string>
#include "NPC.h"
#include "Map.h"
#include "SlowPrint.h"
#include "TicTacToeGame.h" // *** 包含井字棋游戏的头文件 ***
#include "json.hpp"     // *** 新增：JSON库头文件，确保它在你的项目中 ***

#include <vector>
#include <bitset>       // 用于处理位操作
#include <chrono>       // 用于时间/延迟
#include <thread>       // 用于线程休眠
#include <conio.h>    // 为了使用 _kbhit() 和 _getch()
#include <limits>

#include <fstream>      // *** 新增：用于文件读写 ***

using json = nlohmann::json;

#ifdef _WIN32

#include <windows.h>
#endif

// 帮助函数：设置控制台文本颜色
void SetConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Game::clearScreen() const {
#ifdef _WIN32
    system("cls"); // Windows系统使用 "cls"
#else
    system("clear"); // Linux/macOS系统使用 "clear"
#endif
}

// Game构造函数：初始化游戏世界
Game::Game()
    : gameMap(),
    player(gameMap.startRoom),
    mistMonster("迷雾怪物", 200, 25, 10, Item("迷雾核心", "一个强大的能量源", ItemType::CONSUMABLE, ItemEffect::ATTACK_BUFF, 20)),
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
        SetConsoleColor(15); // 白色
        std::cout << "请输入你的选择: ";
        std::cin >> choice;
        std::cout << "\n----------------------------------------\n";

        if (choice == "1") {
            // 当选择1时，进入并开始探索循环
            clearScreen();

            SetConsoleColor(11); // 亮青色
            std::string message;
            message = "无尽的迷雾吞噬了一切，你孤身一人迷失其中。\n记忆模糊，只剩下两个念头：生存，以及找到迷雾的源头并摧毁它。\n你将探索废墟、森林和古老遗迹，与邪恶的生物战斗，寻找散落的线索和强大的装备。\n每一个转角都可能隐藏着未知的危险，或是一线生机。\n你的每一个选择，都将决定你的命运。\n准备好了吗，探险家？\n迷雾的世界在等待着你。\n";

            slowPrint(message, 1);

            message = "在探索模式中，你要尽可能多地获得物品、使用物品以提升自己的实力。你随时可以挑战【迷雾怪物】，来获取【逃脱】，前提是要保障自己实力足够强大。\n";
            slowPrint(message, 1);

            SetConsoleColor(15); // 白色

            player.showStatus();

            std::cout << "\n(按回车键继续...)\n";
            std::cin.get();

            explorationLoop();
            // 从探索循环退出后，清屏并重新显示Logo，回到主菜单
            clearScreen();
            showLogo();
        }
        else if (choice == "2") {
            clearScreen();
            showCommands();
        }
        else if (choice == "3") {
            isRunning = false;
            SetConsoleColor(12); // 亮红色
            std::cout << "感谢游玩《Mist Monster》。\n";
            SetConsoleColor(15); // 白色
        }
        /*else if (choice == "4") {
            clearScreen();
            challengeMonster();
        }*/

        else if (choice == "4") {
            if (loadGame()) {
                std::cout << "存档加载成功！\n";
                explorationLoop(); // 加载成功后直接进入探索
                // 从探索循环退出后，清屏并重新显示Logo，回到主菜单
                clearScreen();
                showLogo();
            }
            else {
                std::cout << "加载失败，存档文件不存在或已损坏。\n";
            }
        }
        else if (choice == "5") {
            saveGame();
            std::cout << "游戏进度已保存。\n";
        }

        else {
            SetConsoleColor(4); // 红色
            std::cout << "无效的输入，请重新选择。\n";
            SetConsoleColor(15); // 白色
        }
        std::cout << "----------------------------------------\n\n";
    }
}

void Game::showLogo() const {
    SetConsoleColor(14); // 亮黄色
    std::cout << "########################################\n";
    std::cout << "#                                      #\n";
    std::cout << "#             Mist Monster             #\n";
    std::cout << "#                                      #\n";
    std::cout << "########################################\n\n";
    SetConsoleColor(11); // 亮青色
    std::cout << "欢迎来到《Mist Monster》！\n";
    std::cout << "在这个充满迷雾的世界中，你需要探索发育，击败迷雾怪物并生存下去。\n";
    SetConsoleColor(15); // 白色
}

void Game::showMainMenu() {
    std::cout << "1. 开始新游戏，进入探索发育模式\n";
    std::cout << "2. 查看游戏说明\n";
    std::cout << "3. 退出游戏\n";
    //std::cout << "4. 挑战迷雾怪物\n";
    std::cout << "4. 继续游戏 (加载存档)\n";
    std::cout << "5. 保存进度（以便下次从当前状态开始）\n"; // 添加保存选项
}

void Game::explorationLoop() {
    bool isExploring = true;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	clearScreen();
    // 第一次进入时，先进行一次房间互动检查（例如，如果起始房间有敌人）
    handleRoomInteraction();


    while (isExploring && player.isAlive()) {
        // 清屏
        clearScreen();

        // 2. 显示当前环境和状态
        SetConsoleColor(11); // 亮青色
        std::cout << "--- 探索模式 ，如要结束探索 menu 可返回主菜单，在主菜单中可保存进度---\n";
        SetConsoleColor(15); // 白色
		std::cout << "你现在在: " << player.currentRoom->getName() << "\n";
		player.currentRoom->look();
        std::cout << std::endl;
        //player.showStatus();
        std::cout << "----------------\n";

        // 3. 提示并获取玩家指令
        SetConsoleColor(10); // 亮绿色
        std::cout << "通过输入以下指令进行探索发育，look观察周围环境，触发遭遇！（help 查看指令说明）：\n";   
        std::cout << "'look', 'go [direction]' , 'take [item]' , 'status' , 'inv', 'use [你的物品]' , 'map',  'Mist' , 'menu' , 'help'): \n>";
        SetConsoleColor(15); // 白色




        std::string command;
        // 使用 std::cin.ignore() 和 std::getline 来读取带空格的完整指令

        std::getline(std::cin, command);

        // 4. 处理玩家指令
        if (command == "menu" || command == "quit" || command == "exit") {
            isExploring = false;
            SetConsoleColor(9); // 亮蓝色
            std::cout << "正在返回主菜单...\n";
            SetConsoleColor(15); // 白色
            // 暂停一下让玩家看到信息
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else {
            processExplorationInput(command);

            // 每次行动后，都检查是否触发了房间互动（如进入有怪的房间）  go 指令------------------------------------------------------->修复NPC对话反复弹出问题
			if (command.find("go ") != std::string::npos || command == "look")
            {
				clearScreen();
                SetConsoleColor(11); // 亮青色
                std::cout << "--- 探索模式 ，如要结束探索 menu 可返回主菜单，在主菜单中可保存进度---\n";
                SetConsoleColor(15); // 白色
                std::cout << "你现在在: " << player.currentRoom->getName() << "\n";
                player.currentRoom->look();
                std::cout << std::endl;
                //player.showStatus();
                std::cout << "----------------\n";

                handleRoomInteraction();


             }
            
        }
		
   
    }

    // 如果玩家死亡，则重置游戏状态
    if (!player.isAlive()) {
        SetConsoleColor(12); // 亮红色
        std::cout << "\n你已在探索中倒下...按任意键返回主菜单。\n";
        SetConsoleColor(15); // 白色
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

        if (argument == "n") {
			argument = "north";
		}
		else if (argument == "s") { argument = "south"; }
		else if (argument == "e") { argument = "east"; }
        else if (argument == "w") {
            argument = "west";
        }

        player.move(argument, this->gameMap);
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else if (command == "map") {
        gameMap.printMap(); // 从 Map::printMap() 改为 gameMap.printMap()
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get();
    }
    else if (command == "look") {
        // 'look' 的功能已在循环开始时自动执行
        player.currentRoom->look();
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else if (command == "status") {
        // 'status' 的功能也已自动执行，但同样允许手动查看
        player.showStatus();
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else if (command == "inventory" || command == "inv") {
        player.showInventory();
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else if (command == "use") {
        player.useItem(argument);
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else if (command == "take" || command == "get") {
        if (argument.empty()) {
            std::cout << "你想拾取什么？\n";
        }
        else {
            // 从当前房间尝试移除物品
            Item* itemToTake = player.currentRoom->removeItem(argument);

            if (itemToTake != nullptr) {
                // 如果成功，将物品添加到玩家背包
                player.takeItem(*itemToTake);
                // 重要：因为物品已经复制到玩家背包，需要释放原始物品的内存
                delete itemToTake;
            }
            else {
                // 如果失败，告知玩家
                std::cout << "这里没有 '" << argument << "\n";
            }
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }

    else if (command == "Mist" || command == "mist") {
        slowPrint("你感受到一股神秘的力量在召唤你去挑战迷雾怪物……\n", 50);
        slowPrint("如果你觉得自己已经足够强大，可以选择挑战它！\n", 50);
        player.showStatus();
        player.showInventory();

        slowPrint("你确定要现在就挑战迷雾怪物吗？ (y/n): ", 50);
        char choice;
        std::cin >> choice;
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 清除输入缓冲区
        if (choice == 'y' || choice == 'Y') {
            challengeMonster();
        }
        else {
            std::cout << "你决定暂时不挑战迷雾怪物，继续你的探索发育之旅。\n";
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }

    else if (command == "help") {
        showCommands();
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
    else {
        std::cout << "未知的指令。输入 help 寻求帮助。\n";
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }

}


void Game::handleRoomInteraction() {
    Room* room = player.currentRoom;

    // 遇敌
    if (room->enemy != nullptr) {
        SetConsoleColor(12); // 亮红色
        std::cout << "\n你遇到了 " << room->enemy->name << "!\n";
        SetConsoleColor(15); // 白色
        BattleSystem battle;
        BattleResult result = battle.startBattle(player, *room->enemy); // 接收新的返回值


        switch (result) {
        case BattleResult::WIN: {
            SetConsoleColor(10); // 亮绿色
            std::cout << "你击败了 " << room->enemy->name << "!\n";
            Item droppedLoot = room->enemy->loot;
            if (!droppedLoot.name.empty()) { // 检查是否有掉落物
                std::cout << "你获得了物品: " << droppedLoot.name << ".\n";
                player.takeItem(droppedLoot);
            }
            delete room->enemy;
            room->enemy = nullptr;
            SetConsoleColor(15); // 白色
            std::cout << "\n(按回车键继续...)\n";
            std::cin.get();
            break;
        }
        case BattleResult::LOSS: {
            SetConsoleColor(4); // 红色
            // 游戏结束的逻辑已经在主探索循环中处理，这里可以留空或只显示消息
            std::cout << "你被击败了...\n";

            SetConsoleColor(15); // 白色
            std::cout << "\n(按回车键继续...)\n";
            std::cin.get();
            break;
        }
        case BattleResult::FLED: {
            SetConsoleColor(9); // 亮蓝色
            std::cout << "你回到了" << player.currentRoom->getName() << "心有余悸。\n";
            
            SetConsoleColor(15); // 白色
            std::cout << "\n(按回车键继续...)\n";
            std::cin.get();
            // 逃跑成功，不做任何事，直接回到探索循环
            break;
        }
        }
    }

    // 遇NPC
    if (room->npc != nullptr) {

        // *** 针对柯洁的特殊逻辑 ***
        if (room->npc->name == "柯洁") {
            if (!room->npc->hasGivenItem) {
                room->npc->talk();
                std::cout << "你是否要接受挑战？ (y/n)\n> ";
                char choice;
                std::cin >> choice;
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (choice == 'y' || choice == 'Y') {
                    TicTacToeGame game;
                    if (game.playGame()) { // 如果玩家赢了
                        std::cout << "柯洁赞许地看着你：“不错，这是你应得的奖励。”\n";
                        player.takeItem(gameMap.getItemFromDatabase("阿尔法狗"));
                        player.health += 10;
						player.money += 100;
                        if (player.health > player.maxHealth) player.health = player.maxHealth;
                        std::cout << "你的生命值恢复了10点！并从【柯洁】手里获取了金钱 100！\n";
                        room->npc->hasGivenItem = true; // 标记奖励已给出
                    }
                    else {
                        std::cout << "柯洁摇了摇头：“看来你还需多加磨练。”\n";
                    }
                }
            }
            else {
                std::cout << "柯洁说：“我们已经分出胜负了。”\n";
            }
        }
        // *** 其他普通NPC的逻辑 ***

        else{
            SetConsoleColor(9); // 亮蓝色
            room->npc->talk();
            if (!room->npc->hasGivenItem) {
                Item receivedItem = room->npc->giveItem();
                std::cout << "你从 " << room->npc->name << " 那里获得了: " << receivedItem.name << "!\n";
                player.takeItem(receivedItem);
            }
            SetConsoleColor(15); // 白色
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get();
    }

    //饭店
    if (room->description.find("金碧辉煌的饭店") != std::string::npos) {
        SetConsoleColor(14); // 亮黄色
        std::cout << "主理人：欢迎光临！要来点吃的恢复体力吗？ (y/n)\n";
        SetConsoleColor(15); // 白色
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            int cost = rand() % 20 + 10; // 随机价格 10-29
            std::cout << "这顿饭需要 " << cost << " 。\n";
            std::cout << "\n(按回车键继续...)\n";
            std::cin.get(); // 等待玩家按回车

            if (player.money >= cost) {
                player.money -= cost;
                player.stamina = player.maxStamina;
				player.health = player.maxHealth;
				//TODO:提示玩家金钱变化
                std::cout << "你支付了 " << cost << " 剩余金钱：" << player.money << std::endl;
                SetConsoleColor(10); // 亮绿色
                std::cout << "你吃得心满意足，体力完全恢复了！并恢复了所有生命值！\n";
                SetConsoleColor(15); // 白色
            }
            else {
                SetConsoleColor(4); // 红色
				std::cout << "你只有" << player.money << "，钱不够付饭钱！\n";
                std::cout << "\n(按回车键继续...)\n";
                std::cin.get(); // 等待玩家按回车

                std::cout << "主理人把你打了一顿！\n";
                player.takeDamage(10); // 吃霸王餐被打
                player.stamina = player.maxStamina;

                std::cout << "\n(按回车键继续...)\n";
                std::cin.get(); // 等待玩家按回车

                SetConsoleColor(10); // 亮绿色
                std::cout << "虽然挨了打，但你还是吃饱了，体力恢复了。\n";
                SetConsoleColor(15); // 白色
            }
            
        }
        else if(choice == 'n' || choice == 'N') {
			std::cout << "你决定不吃饭，继续你的探索。\n";
        }
        else {
			std::cout << "主理人看你一眼，觉得你有点怪，什么也没发生。\n";
        }
        std::cout << "\n(按回车键继续...)\n";
        std::cin.get(); // 等待玩家按回车
    }
}



void Game::showCommands() const {
    SetConsoleColor(11); // 亮青色
    std::cout << "\n探索模式指令:\n";
    SetConsoleColor(15); // 白色
	std::cout << "--------------------\n";
    std::cout << "在这个游戏中，你需要在探索模式中不断精进自己，只有击败最终的迷雾怪物，才能【逃脱】，否则【迷失】\n";
	std::cout << "你可以使用以下指令来进行探索和互动：\n";
    std::cout << "  go [north/south/east/west/n/s/w/e]: 向指定方向移动。\n";
    std::cout << "  take [物品名]: 拾取地上的物品。\n";
    std::cout << "  look: 查看当前环境。在每到一个新的环境，你【必须】look观察环境来获取信息，包括【你可以走的方向】【你可能获得的物品】【你可能遭遇的事件】\n"; // 将 look 的描述修正
    std::cout << "  status: 查看你的当前状态。\n";
    std::cout << "  inventory (或 inv): 查看你的背包。\n";
    std::cout << "  use [物品名]: 使用背包中的一个物品。\n";
    std::cout << "  map: 查看世界地图。\n";
	std::cout << "  Mist: 挑战迷雾怪物（需要谨慎）。\n";
    std::cout << "  menu: 返回主菜单。\n\n";

    std::cout << "战斗时指令：\n";
	std::cout << "--------------------\n";
	std::cout << "  attack/a: 普通攻击敌人。\n";
    std::cout << "  run/r：尝试逃跑，但有概率失败。\n";
	std::cout << "  use [物品名]: 使用背包中的一个物品。\n";
    std::cout << "  retain/re，蓄力准备释放大招，但你需要静止一回合。\n";
    std::cout << "  bang/b，释放大招对敌人造成巨大伤害\n";
    std::cout << "  help/h，显示指令帮助信息\n\n";

    std::cout << "  部分相关指令有对应快捷缩写，可减少输入指令的时间。\n";
    std::cout << "  请仔细阅读并牢记指令，以免战场查看指令时错失攻击回合。\n";


	//继续补充游戏机制
    std::cout << "金碧辉煌的饭店：只要你同意主理人在此用餐，无论钱够不够都会恢复满体力（不给钱只是会有一些代价），但只有你能付够饭钱时，才能恢复满生命值。";

    std::cout << "当玩家武器熟练后，触发大招，大招在战斗时需要一个回合的时间积蓄力量，请合理分配资源\n" << std::endl;
    std::cout << "--------------------\n";
}

void Game::challengeMonster() {
    SetConsoleColor(12); // 亮红色
    slowPrint("你鼓起勇气，决定挑战最终的迷雾怪物！挑战成功，则逃脱迷雾，若失败，则永久迷失……\n", 30);
    SetConsoleColor(15); // 白色
    BattleSystem battle;
    BattleResult result = battle.startBattle(player, mistMonster);

    if (result == BattleResult::WIN) {
        SetConsoleColor(14); // 亮黄色
        std::cout << "\n****************************************\n";
        std::cout << "你成功击败了迷雾怪物！浓雾散去，你找到了出路。\n";
        std::cout << "结局：【逃脱】\n";
        std::cout << "****************************************\n";
        SetConsoleColor(15); // 白色
        isRunning = false; // 游戏结束
    }
    else {
        SetConsoleColor(4); // 红色
        std::cout << "\n****************************************\n";
        std::cout << "迷雾吞噬了你，你永远地迷失在了这里。\n";
        std::cout << "结局：【迷失】\n";
        std::cout << "****************************************\n";
        SetConsoleColor(15); // 白色
        isRunning = false; // 游戏结束
    }
}

void Game::resetGame() {
    std::cout << "\n...回到了初始的界面...\n\n";
    // 创建一个新的Player对象来重置状态
    player = Player(gameMap.startRoom);
}

// *** 新增：实现 saveGame ***
void Game::saveGame() const {
    json saveData;

    // 1. 保存玩家数据
    saveData["player"]["name"] = player.name;
    saveData["player"]["health"] = player.health;
    saveData["player"]["maxHealth"] = player.maxHealth;
    saveData["player"]["attack"] = player.attack;
    saveData["player"]["defense"] = player.defense;
    saveData["player"]["agility"] = player.agility;
    saveData["player"]["intelligence"] = player.intelligence;
    saveData["player"]["stamina"] = player.stamina;
    saveData["player"]["maxStamina"] = player.maxStamina;
    saveData["player"]["money"] = player.money;
    saveData["player"]["weaponProficiency"] = player.weaponProficiency;
    saveData["player"]["currentRoomName"] = player.currentRoom->name;

    for (const auto& item : player.inventory) {  //背包
        saveData["player"]["inventory"].push_back(item.name);
    }
	if (!player.equippedWeapon.name.empty()) {  //装备
        saveData["player"]["equippedWeapon"] = player.equippedWeapon.name;
    }

    // 2. 保存世界状态（哪些敌人被击败，哪些物品被拾取）
    json worldState;
    for (const auto& room : gameMap.allRooms) {
        // 记录没有敌人的房间（假设它们原来有）
        if (room->enemy == nullptr) {
            worldState[room->name]["hasEnemy"] = false;
        }
        // 记录房间里还剩下哪些物品
        std::vector<std::string> remainingItems;
        for (const auto& itemPtr : room->items) {
            if (itemPtr) remainingItems.push_back(itemPtr->name);
        }
        worldState[room->name]["items"] = remainingItems;
    }
    saveData["worldState"] = worldState;

    // 3. 写入文件
    std::ofstream file("savegame.json");
    file << saveData.dump(4); // dump(4) 格式化输出，便于查看
    file.close();
}
// *** 新增：实现 loadGame ***
bool Game::loadGame() {
    std::ifstream file("savegame.json");
    if (!file.is_open()) {
        std::cout << "无法打开存档文件。\n";
        return false; // 文件不存在或无法打开

    }
    json saveData;
    try {
        file >> saveData;
    }
    catch (json::parse_error& e) {
        std::cerr << "存档文件解析错误: " << e.what() << '\n';
        return false;
    }
    file.close();

    // 1. 首先，构建一个全新的、完整的世界
    gameMap.build();

    // 2. 加载玩家数据并覆盖
    player.name = saveData["player"]["name"];
    player.health = saveData["player"]["health"];
    player.maxHealth = saveData["player"]["maxHealth"];
    player.attack = saveData["player"]["attack"];
    player.defense = saveData["player"]["defense"];
    player.agility = saveData["player"]["agility"];
    player.intelligence = saveData["player"]["intelligence"];
    player.stamina = saveData["player"]["stamina"];
    player.maxStamina = saveData["player"]["maxStamina"];
    player.money = saveData["player"]["money"];
    player.weaponProficiency = saveData["player"]["weaponProficiency"];
    // 3. 恢复玩家位置
    std::string roomName = saveData["player"]["currentRoomName"];
    bool roomFound = false;
    for (auto* room : gameMap.allRooms) {
        if (room->name == roomName) {
            player.currentRoom = room;
            roomFound = true;
            break;
        }
    }
    if (!roomFound) return false; // 存档中的房间名无效

    // 4. 恢复玩家背包和装备
    player.inventory.clear();
    for (const std::string& itemName : saveData["player"]["inventory"]) {
        player.inventory.push_back(gameMap.getItemFromDatabase(itemName));
    }
    if (saveData["player"].contains("equippedWeapon")) {
        player.equippedWeapon = gameMap.getItemFromDatabase(saveData["player"]["equippedWeapon"]);
    }

    // 5. 根据存档，“修复”世界状态
    const json& worldState = saveData["worldState"];
    for (auto& room : gameMap.allRooms) {
        if (worldState.contains(room->name)) {
            // 如果存档记录这个房间没有敌人，就删除它
            if (worldState[room->name]["hasEnemy"] == false && room->enemy != nullptr) {
                delete room->enemy;
                room->enemy = nullptr;
            }
            // ***【开始同步物品】***
            // 1. 清空房间里由 build() 放置的初始物品，并释放内存
            for (Item* itemPtr : room->items) {
                delete itemPtr;
            }
            room->items.clear();

            // 2. 读取存档中该房间应有的物品列表
            std::vector<std::string> savedItems = worldState[room->name]["items"];

            // 3. 根据列表，重新创建并添加物品到房间
            for (const std::string& itemName : savedItems) {
                // 从数据库获取物品原型，并为其创建新的动态内存实例
                Item* newItem = new Item(gameMap.getItemFromDatabase(itemName));
                room->takeItem(newItem); // 使用 takeItem (即 addItem) 将新物品放入房间
            }
            // ***【物品同步结束】***
            
        }
    }

    return true;


}