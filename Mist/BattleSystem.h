#pragma once

// 前向声明
class Player;
class Enemy;
class Game;

class BattleSystem {
private:
    Game* mediator_ = nullptr;
    Player* player_ = nullptr;
    Enemy* enemy_ = nullptr;
    bool inBattle_ = false;

public:
    BattleSystem() = default;

    void setMediator(Game* game) { mediator_ = game; }

    /**
     * @brief 开始一场战斗，这是战斗系统的入口点
     * @param player 指向玩家的指针
     * @param enemy 指向敌人的指针
     */
    void startBattle(Player* player, Enemy* enemy);

private:
    /**
     * @brief 战斗主循环，处理回合交替
     */
    void battleLoop();

    /**
     * @brief 处理玩家的回合
     */
    void playerTurn();

    /**
     * @brief 处理敌人的回合
     */
    void enemyTurn();

    /**
     * @brief 在每个回合开始时显示状态信息
     */
    void displayStatus() const;

    /**
     * @brief 战斗结束，清理状态并向中介者报告结果
     */
    void endBattle();
};