#include "BattleSystem.h"
#include "Player.h"
#include "Enemy.h"
#include "SlowPrint.h"
#include <iostream>
#include <string>
#include <limits>
#include <random> // 用于概率计算


// 用于跟踪玩家是否选择逃跑并成功
bool playerFled = false;

BattleResult BattleSystem::startBattle(Player& player, Enemy& enemy) {
    std::cout << "--- 战斗开始! ---\n";
    player.isUltimateCharged = false; // 每次战斗开始时重置蓄力状态
    playerFled = false; // 重置逃跑状态

    while (player.isAlive() && enemy.isAlive() && !playerFled) {
        playerTurn(player, enemy);
        if (!enemy.isAlive()) break; // 玩家胜利
        if (playerFled) break;      // 玩家成功逃跑

        enemyTurn(player, enemy);
    }

    std::cout << "--- 战斗结束! ---\n";

    if (playerFled) {
        return BattleResult::FLED;
    }
    else if (player.isAlive()) {
        return BattleResult::WIN;
    }
    else {
        return BattleResult::LOSS;
    }
}


void BattleSystem::playerTurn(Player& player, Enemy& enemy) {
    std::cout << "\n--- 你的回合 ---\n";
    player.showStatus();
    std::cout << "敌人: " << enemy.name << " | 生命: " << enemy.health << "\n";

    // 根据是否蓄力显示不同的指令
    if (player.isUltimateCharged) {
        std::cout << "魂灵已附身武器！输入 'bang' 释放大招，或 'use' 使用物品。\n> ";
    }
    else {
        std::cout << "输入指令: 'attack', 'run', 'use' [物品名], help(查看指令介绍手册)";
        if (player.weaponProficiency >= 5) {
            std::cout << ", 'retain'";
        }
        std::cout << "\n> ";
    }

    std::string input;
    std::string command;
    std::string argument;
    std::getline(std::cin, input);

    size_t space_pos = input.find(' ');
    if (space_pos != std::string::npos) {
        command = input.substr(0, space_pos);
        argument = input.substr(space_pos + 1);
    }
    else {
        command = input;
    }

    if (player.isUltimateCharged) {
        if (command == "bang") {
            slowPrint("死魂灵的力量从武器中喷涌而出！\n", 50);
            // 大招伤害计算
            int baseDamage = player.calculateAttack() - enemy.defense;
            int ultimateDamage = static_cast<int>(baseDamage * 2.0 + player.intelligence * 1.5);
            if (ultimateDamage < 0) ultimateDamage = 1;

            enemy.takeDamage(ultimateDamage);
            std::cout << "你对 " << enemy.name << " 造成了 " << ultimateDamage << " 点毁灭性伤害！\n";
            player.isUltimateCharged = false; // 释放后重置状态
        }
        else if (command == "use") {
            player.useItem(argument);
        }
        else {
            std::cout << "指令错误！你犹豫了一下，强大的能量消散了。\n";
            player.isUltimateCharged = false; // 错误指令导致蓄力失败
        }
    }
    else {
        if (command == "attack") {
            // 新的普通攻击伤害计算
            int damage = player.calculateAttack() - enemy.defense;
            if (damage < 0) damage = 0;

            // 暴击判定
            int critChance = player.agility; // 敏捷越高，暴击率越高
            if ((rand() % 100) < critChance) {
                damage *= 2;
                std::cout << "暴击！";
            }

            enemy.takeDamage(damage);
            std::cout << "你对 " << enemy.name << " 造成了 " << damage << " 点伤害。\n";
        }
        else if (command == "run") {
            std::cout << "你尝试逃离战斗...\n";
            int fleeChance = (player.agility * 10) / (enemy.attack + 1);
            if ((rand() % 100) < fleeChance) {
                std::cout << "你成功逃跑了！\n";
                playerFled = true;
            }
            else {
                std::cout << "逃跑失败！你在慌乱中步伐不稳。\n";
                player.agility -= 1; // 逃跑失败惩罚
                if (player.agility < 1) player.agility = 1;
                std::cout << "你的敏捷度降低了！\n";
            }
        }
        else if (command == "retain" && player.weaponProficiency >= 5) {
            std::cout << "你集中精神，开始引导死魂灵的力量...\n";
            player.isUltimateCharged = true;
        }
        else if (command == "use") {
            player.useItem(argument);
        }
        else if (command == "help") {
            std::cout << "可用指令:\n";
            std::cout << "  attack          普通攻击敌人\n";
            std::cout << "  run             尝试逃跑\n";
            std::cout << "  use [物品名]    使用背包中的物品\n";
            if (player.weaponProficiency >= 5) {
                std::cout << "  retain         蓄力准备释放大招，但你需要静止一回合。\n";
            }
            if (player.isUltimateCharged) {
                std::cout << "  bang           释放大招对敌人造成巨大伤害\n";
            }
            std::cout << "  help           显示此帮助信息\n";
        }
        else {
            std::cout << "无效的输入，你犹豫了一下，失去了机会。\n";
        }
    }
}


void BattleSystem::enemyTurn(Player& player, Enemy& enemy) {
    std::cout << "\n--- 敌人回合 ---\n";

    // 敌人攻击命中判定
    int hitChance = 100 - (player.agility * 2); // 玩家敏捷越高，敌人越容易miss
    if ((rand() % 100) < hitChance) {
        int damage = enemy.attack - player.defense;
        if (damage < 0) damage = 0;
        player.takeDamage(damage);
        std::cout << enemy.name << " 对你造成了 " << damage << " 点伤害。\n";
    }
    else {
        std::cout << enemy.name << " 的攻击被你灵巧地躲开了！\n";
    }
}