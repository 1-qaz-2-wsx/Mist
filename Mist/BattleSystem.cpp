#include "BattleSystem.h"
#include "Player.h"
#include "Room.h"


#include <iostream>

std::vector<unsigned> BattleSystem::fight(Player& player, Room& room) {
    std::vector<unsigned> totalLoot;

    auto enemies = room.getEnemys(); // 这里只拿 ID（最薄依赖）

    for (auto enemyId : enemies) {
        // === 这里通常要从 EnemyDatabase 取敌人原型，这里简化演示 ===
        Enemy e("Slime", enemyId, /*hp*/30, /*atk*/5, /*drops*/{ 101 });

        std::cout << "遭遇敌人#" << e.getId() << " [" << e.getName() << "]\n";
        while (!e.isDead() && !player.isDead()) {
            e.takeDamage(player.atk());
            if (!e.isDead()) player.takeDamage(e.atk());
        }
        if (player.isDead()) {
            std::cout << "你倒下了……\n";
            break;
        }
        std::cout << "击败敌人#" << e.getId() << "\n";
        totalLoot.insert(totalLoot.end(), e.drops().begin(), e.drops().end());
        // 是否从房间中移除该敌人由中介者/或 Room 自身提供 remove 接口处理
    }

    return totalLoot;
}