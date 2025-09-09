// Game.cpp

#include "Game.h"

// ----------------------------------------------------------------
// 包含所有“同事类”和工具类的头文件
// ----------------------------------------------------------------
// 在.cpp文件中，我们需要包含所有需要进行交互的类的完整定义。
#include "Player.h"
#include "Map.h"
#include "BattleSystem.h"
#include "Room.h"
#include "ItemDatabase.h"
#include "EnemyDatabase.h"
#include "RoomDatabase.h"

#include <iostream>
#include <string>
#include <sstream> // 用于解析字符串输入

// --- 构造/析构函数 ---

Game::Game() {
    // 构造函数可以保持为空，真正的初始化在 initialize() 中进行
    // unique_ptr 默认会初始化为 nullptr
}

// 析构函数必须在.cpp中实现，因为头文件中只有前向声明
// 在这里，编译器看到了所有类的完整定义，知道如何正确销毁 unique_ptr
Game::~Game() {
}

// --- 游戏主流程控制 ---

bool Game::initialize(const std::string& dataFolderPath) {
    std::cout << "正在初始化游戏..." << std::endl;

    try {
        // 1. 创建并加载数据库 (顺序很重要，因为其他系统依赖它们)
        itemDB_ = std::make_unique<ItemDatabase>();
        itemDB_->load(dataFolderPath + "/items.json");

        enemyDB_ = std::make_unique<EnemyDatabase>();
        enemyDB_->load(dataFolderPath + "/enemies.json");

        roomDB_ = std::make_unique<RoomDatabase>();
        roomDB_->load(dataFolderPath + "/rooms.json", itemDB_.get(), enemyDB_.get());

        // 2. 创建地图并构建世界
        map_ = std::make_unique<Map>();
        map_->buildFromDatabase(roomDB_.get()); // 使用.get()从unique_ptr获取裸指针

        // 3. 创建玩家并设置初始位置
        player_ = std::make_unique<Player>(); // 你可以在Player构造函数中设置初始属性
        player_->setCurrentRoomId(map_->getStartingRoomId());

        // 4. 创建战斗系统
        battleSystem_ = std::make_unique<BattleSystem>();

        // 5. [关键] 将中介者(Game* this)注入需要它的同事类
        // 注意: 你需要在 BattleSystem 和 Player 类中添加 setMediator(Game* game) 方法
        battleSystem_->setMediator(this);
        // player_->setMediator(this); // 如果Player也需要回调Game，也添加这行

    }
    catch (const std::exception& e) {
        // 如果任何步骤失败（例如文件找不到、JSON格式错误），则初始化失败
        std::cerr << "初始化失败: " << e.what() << std::endl;
        return false;
    }

    isRunning_ = true;
    std::cout << "初始化成功！" << std::endl;
    return true;
}

void Game::run() {
    if (!isRunning_) {
        std::cout << "游戏未初始化。" << std::endl;
        return;
    }

    std::cout << "\n欢迎来到 MUD 世界！" << std::endl;
    std::cout << "输入 'help' 查看可用指令。" << std::endl;

    // 触发进入初始房间的逻辑
    enterRoom(player_->getCurrentRoomId());

    while (isRunning_) {
        gameLoop();
    }

    std::cout << "感谢游玩，再见！" << std::endl;
}


// --- 中介者接口实现 ---

void Game::notifyBattleFinished(bool playerWon, const std::vector<unsigned int>& lootItemIds) {
    std::cout << "\n[系统] 战斗结束。" << std::endl;
    // 战斗结束后，调用内部逻辑处理后续事宜
    processBattleAftermath(playerWon, lootItemIds);
}


// --- 内部逻辑与流程处理 ---

void Game::gameLoop() {
    std::cout << "\n> "; // 显示输入提示符
    processPlayerInput();
}

void Game::processPlayerInput() {
    std::string line;
    std::getline(std::cin, line);

    // 使用 stringstream 来轻松分割命令和参数
    std::stringstream ss(line);
    std::string command;
    ss >> command;

    // 将命令转为小写，方便比较
    for (char& c : command) {
        c = tolower(c);
    }

    // 根据命令分发到不同的处理函数
    if (command == "go" || command == "move") {
        std::string direction;
        ss >> direction;
        handleMoveCommand(direction);
    }
    else if (command == "look" || command == "l") {
        handleLookCommand();
    }
    else if (command == "attack") {
        std::string target;
        ss >> target;
        handleAttackCommand(target);
    }
    else if (command == "take" || command == "get") {
        std::string item;
        ss >> item;
        handleTakeCommand(item);
    }
    else if (command == "inventory" || command == "inv" || command == "i") {
        handleInventoryCommand();
    }
    else if (command == "help") {
        handleHelpCommand();
    }
    else if (command == "quit" || command == "exit") {
        handleQuitCommand();
    }
    else {
        std::cout << "未知指令。输入 'help' 查看帮助。" << std::endl;
    }
}

void Game::displayCurrentState() const {
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (!currentRoom) {
        std::cerr << "错误：玩家当前房间无效！" << std::endl;
        return;
    }

    // 调用Room自己的方法来显示信息 (这是Room类的职责)
    currentRoom->display();
}

void Game::enterRoom(unsigned int newRoomId) {
    player_->setCurrentRoomId(newRoomId);
    std::cout << "\n----------------------------------------" << std::endl;
    handleLookCommand(); // 进入新房间后自动执行 "look"

    // 检查房间内是否有敌人，并可能触发战斗
    checkForEnemiesAndInitiateBattle();
}


// --- 玩家指令处理函数 ---

void Game::handleMoveCommand(const std::string& direction) {
    if (direction.empty()) {
        std::cout << "你要往哪个方向走？(north, south, east, west...)" << std::endl;
        return;
    }

    unsigned int currentRoomId = player_->getCurrentRoomId();
    Room* currentRoom = map_->getRoom(currentRoomId);

    // 向Map系统查询出口信息
    int nextRoomId = currentRoom->getExit(direction);

    if (nextRoomId != -1) { // -1 代表没有出口
        std::cout << "你向 " << direction << " 方向走去..." << std::endl;
        enterRoom(static_cast<unsigned int>(nextRoomId));
    }
    else {
        std::cout << "这个方向没有路。" << std::endl;
    }
}

void Game::handleLookCommand() const {
    displayCurrentState();
}

void Game::handleAttackCommand(const std::string& targetName) {
    if (targetName.empty()) {
        std::cout << "你要攻击谁？" << std::endl;
        return;
    }

    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (!currentRoom || currentRoom->enemies.empty()) {
        std::cout << "这里没有敌人。" << std::endl;
        return;
    }

    // 查找敌人
    Enemy* target = nullptr;
    for (const auto& enemy : currentRoom->enemies) {
        if (enemy->getName() == targetName) { // 这里可以用更灵活的匹配
            target = enemy.get();
            break;
        }
    }

    if (target) {
        // [中介者模式] Game命令BattleSystem开始战斗
        battleSystem_->startBattle(player_.get(), target);
    }
    else {
        std::cout << "这里没有名叫 " << targetName << " 的敌人。" << std::endl;
    }
}

void Game::handleTakeCommand(const std::string& itemName) {
    if (itemName.empty()) {
        std::cout << "你要拾取什么？" << std::endl;
        return;
    }
    // TODO: 实现拾取逻辑
    // 1. 获取当前房间
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (!currentRoom) return;

    // 2. 尝试从房间拿出物品 (中介者协调)
    std::unique_ptr<Item> item = currentRoom->takeItem(itemName);

    // 3. 如果成功，则交给玩家 (中介者协调)
    if (item) {
        player_->addItemToInventory(std::move(item));
    }
    else {
        std::cout << "地上没有这个东西。" << std::endl;
    }
}

void Game::handleInventoryCommand() const {
    player_->displayInventory();
    //std::cout << "你的背包是空的（物品栏系统尚未实现）。" << std::endl;
}

void Game::handleHelpCommand() const {
    std::cout << "--- 可用指令 ---\n"
        << "go [direction] - 向指定方向移动 (e.g., go north)\n"
        << "look           - 查看当前环境\n"
        << "attack [name]  - 攻击指定敌人\n"
        << "take [item]    - 拾取地上的物品\n"
        << "inventory      - 查看你的背包\n"
        << "quit           - 离开游戏\n"
        << "-----------------" << std::endl;
}

void Game::handleQuitCommand() {
    std::cout << "你确定要离开游戏吗？(yes/no)" << std::endl;
    std::string confirmation;
    std::cin >> confirmation;
    // 清理cin的缓冲区
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirmation == "yes" || confirmation == "y") {
        isRunning_ = false;
    }
    else {
        std::cout << "你决定继续冒险。" << std::endl;
    }
}


// --- 内部事件处理 ---

void Game::checkForEnemiesAndInitiateBattle() {
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    // 简单设计：如果房间里有敌人，自动触发与第一个敌人的战斗
    if (currentRoom && !currentRoom->enemies.empty()) {
        std::cout << "\n你遭遇了敌人！" << std::endl;
        // 使用 .get() 从 unique_ptr 获取裸指针
        battleSystem_->startBattle(player_.get(), currentRoom->enemies[0].get());
    }
}

void Game::processBattleAftermath(bool playerWon, const std::vector<unsigned int>& lootItemIds) {
    Room* currentRoom = map_->getRoom(player_->getCurrentRoomId());
    if (!currentRoom) return;

    if (playerWon) {
        std::cout << "你取得了胜利！" << std::endl;
        // 清空房间里的所有敌人 (简化处理)
        currentRoom->enemies.clear();

        // 根据战利品ID，从ItemDatabase创建物品实例并放到地上
        for (unsigned int itemId : lootItemIds) {
            auto lootItem = itemDB_->createInstance(itemId);
            if (lootItem) {
                std::cout << "敌人掉落了 [" << lootItem->getName() << "]。" << std::endl;
                currentRoom->items.push_back(std::move(lootItem));
            }
        }
    }
    else {
        std::cout << "你被击败了... 游戏结束。" << std::endl;
        isRunning_ = false; // 玩家死亡，游戏结束
    }
}