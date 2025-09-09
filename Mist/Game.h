#pragma once
#include <memory>
#include <string>
#include <vector>

//中介者，负责类与类之间的通信。任务是初始化子系统并管理游戏循环。
//前向声明，避免循环引用
class Player;
class Map;
class BattleSystem;
class Room;
class ItemDatabase;
class EnemyDatabase;
class RoomDatabase;

class Game
{

public:
	// --- 构造/析构函数与生命周期管理 ---
	Game();
	~Game();

	// 禁止拷贝和赋值，因为Game是单例资源管理者
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	// --- 游戏主流程控制 ---

	// 初始化所有游戏数据和子系统
	bool initialize(const std::string& dataFolderPath);

	// 启动并运行游戏主循环
	void run();

public:
	// ----------------------------------------------------------------
	// 中介者接口 (Mediator Interface)
	// ----------------------------------------------------------------
	// 这些函数是给“同事类”(如Player, BattleSystem)调用的，用于向中介者报告事件或请求服务。

	/**
	 * @brief 当一场战斗结束时，由BattleSystem调用此函数
	 * @param playerWon 玩家是否胜利
	 * @param lootItemIds 战利品物品的ID列表
	 */
	void notifyBattleFinished(bool playerWon, const std::vector<unsigned int>& lootItemIds);



private:
    // ----------------------------------------------------------------
    // 内部逻辑与流程处理 (Private Helper Methods)
    // ----------------------------------------------------------------
    // 这些是Game类内部用来处理游戏逻辑的私有函数。

    // 游戏主循环，由 run() 调用
    void gameLoop();

    // 处理玩家的输入
    void processPlayerInput();

    // 显示当前房间的信息
    void displayCurrentState() const;

    // 切换玩家所在的房间，并触发进入房间的逻辑
    void enterRoom(unsigned int newRoomId);

    // --- 玩家指令处理函数 ---
    void handleMoveCommand(const std::string& direction);
    void handleLookCommand() const;
    void handleAttackCommand(const std::string& targetName);
    void handleTakeCommand(const std::string& itemName);
    void handleInventoryCommand() const;
    void handleHelpCommand() const;
    void handleQuitCommand();

    // --- 内部事件处理 ---

    // 在进入一个新房间后，检查是否有敌人并触发战斗
    void checkForEnemiesAndInitiateBattle();

    // 处理战斗结束后的各种事宜（如移除敌人、掉落物品）
    void processBattleAftermath(bool playerWon, const std::vector<unsigned int>& lootItemIds);


private:
    // ----------------------------------------------------------------
    // 成员变量 (Member Variables)
    // ----------------------------------------------------------------
    // 中介者持有所有子系统的所有权。使用unique_ptr可以自动管理内存。

    // --- 核心游戏对象 ---
    std::unique_ptr<Player> player_;
    std::unique_ptr<Map> map_;

    // --- 游戏子系统 ---
    std::unique_ptr<BattleSystem> battleSystem_;

    // --- 数据管理器 ---
    std::unique_ptr<ItemDatabase> itemDB_;
    std::unique_ptr<EnemyDatabase> enemyDB_;
    std::unique_ptr<RoomDatabase> roomDB_;

    // --- 游戏状态 ---
    bool isRunning_ = false; // 控制游戏主循环是否继续
};
};


