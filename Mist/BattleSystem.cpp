#include "BattleSystem.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <string>
#include <limits>

bool BattleSystem::startBattle(Player& player, Enemy& enemy) {
    std::cout << "--- 战斗开始! ---\n";

    while (player.isAlive() && enemy.isAlive()) {
        playerTurn(player, enemy);
        if (!enemy.isAlive()) break;
        enemyTurn(player, enemy);
    }

    std::cout << "--- 战斗结束! ---\n";
    return player.isAlive();
}

void BattleSystem::playerTurn(Player& player, Enemy& enemy) {
    std::cout << "\n--- 你的回合 ---\n";
    player.showStatus();
    std::cout << "敌人生命: " << enemy.health << "\n";
    std::cout << "选择行动: (1) 攻击 (2) 使用物品 (3) 逃跑\n> ";

    int choice;
    std::cin >> choice;

    // 清除输入缓冲区
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
    case 1: {
        player.showInventory();
        std::cout << "选择武器 (输入名称): ";
        std::string weaponName;
        std::getline(std::cin, weaponName);
        player.equipWeapon(weaponName);

        int damage = player.calculateAttack() - enemy.defense;
        if (damage < 0) damage = 0;
        enemy.takeDamage(damage);
        std::cout << "你对 " << enemy.name << " 造成了 " << damage << " 点伤害。\n";
        break;
    }
    case 2: {
        player.showInventory();
        std::cout << "输入要使用的物品名称: ";
        std::string itemName;
        std::getline(std::cin, itemName);
        player.useItem(itemName);
        break;
    }
    case 3:
        std::cout << "你决定逃跑...\n";
        // 这里可以添加逃跑成功的几率判定
        break;
    default:
        std::cout << "无效的输入，你犹豫了一下，失去了机会。\n";
        break;
    }
}

void BattleSystem::enemyTurn(Player& player, Enemy& enemy) {
    std::cout << "\n--- 敌人回合 ---\n";
    int damage = enemy.attack - player.defense;
    if (damage < 0) damage = 0;
    player.takeDamage(damage);
    std::cout << enemy.name << " 对你造成了 " << damage << " 点伤害。\n";
}