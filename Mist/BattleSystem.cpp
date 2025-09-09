#include "BattleSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include <iostream>
#include <thread>   // 用于 std::this_thread::sleep_for
#include <chrono>   // 用于 std::chrono::seconds

void BattleSystem::startBattle(Player* player, Enemy* enemy) {
    player_ = player;
    enemy_ = enemy;
    inBattle_ = true;

    std::cout << "\n================================" << std::endl;
    std::cout << "战斗开始！ 你遭遇了 " << enemy_->getName() << "！" << std::endl;
    std::cout << "================================" << std::endl;

    // 直接在此处启动并完成整个战斗循环
    battleLoop();
}

void BattleSystem::battleLoop() {
    while (inBattle_) {
        displayStatus();

        playerTurn();
        if (!enemy_->isAlive()) {
            inBattle_ = false;
            break;
        }

        // 给玩家一点反应时间
        std::this_thread::sleep_for(std::chrono::seconds(1));

        enemyTurn();
        if (!player_->isAlive()) {
            inBattle_ = false;
            break;
        }

        std::cout << "--------------------------------" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 循环结束后，处理战斗结果
    endBattle();
}

void BattleSystem::playerTurn() {
    std::cout << "\n你的回合。目前你只能选择攻击。\n按 Enter 键攻击...";
    // 等待玩家按回车，简化输入处理
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "你发起了攻击！" << std::endl;
    enemy_->takeDamage(player_->stats.attack);
}

void BattleSystem::enemyTurn() {
    std::cout << "\n" << enemy_->getName() << " 的回合。" << std::endl;
    std::cout << enemy_->getName() << " 攻击了你！" << std::endl;
    player_->takeDamage(enemy_->baseStats.attack);
}

void BattleSystem::displayStatus() const {
    std::cout << "\n[状态] "
        << player_->getName() << " HP: " << player_->currentHp << "/" << player_->stats.maxHp
        << "  |  "
        << enemy_->getName() << " HP: " << enemy_->currentHp << "/" << enemy_->baseStats.maxHp
        << std::endl;
}

void BattleSystem::endBattle() {
    bool playerWon = player_->isAlive();

    // [中介者模式]
    // 战斗系统不关心战利品是什么，它只管把战利品ID列表和战斗结果报告给Game
    if (mediator_) {
        if (playerWon) {
            mediator_->notifyBattleFinished(true, enemy_->lootTable);
        }
        else {
            // 如果玩家输了， lootTable 为空
            mediator_->notifyBattleFinished(false, {});
        }
    }

    // 清理战场指针
    player_ = nullptr;
    enemy_ = nullptr;
}