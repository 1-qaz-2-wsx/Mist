#include "BattleSystem.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

bool BattleSystem::startBattle(Player& player, Enemy& enemy) {
    std::cout << "--- 战斗开始! ---\n";

    while (player.isAlive() && enemy.isAlive()) {
        // 玩家回合
        int playerDamage = player.attack - enemy.defense;
        if (playerDamage < 0) playerDamage = 0;
        enemy.takeDamage(playerDamage);
        std::cout << "你对 " << enemy.name << " 造成了 " << playerDamage << " 点伤害。";
        std::cout << enemy.name << " 剩余生命: " << enemy.health << "\n";

        if (!enemy.isAlive()) {
            break; // 敌人被击败
        }

        // 敌人回合
        int enemyDamage = enemy.attack - player.defense;
        if (enemyDamage < 0) enemyDamage = 0;
        player.takeDamage(enemyDamage);
        std::cout << enemy.name << " 对你造成了 " << enemyDamage << " 点伤害。";
        std::cout << "你剩余生命: " << player.health << "\n";
    }

    std::cout << "--- 战斗结束! ---\n";
    return player.isAlive();
}

