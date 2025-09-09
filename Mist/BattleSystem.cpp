#include "BattleSystem.h"
#include "Player.h"
#include "Room.h"


#include <iostream>

std::vector<unsigned> BattleSystem::fight(Player& player, Room& room) {
    std::vector<unsigned> totalLoot;

    auto enemies = room.getEnemys(); // ����ֻ�� ID���������

    for (auto enemyId : enemies) {
        // === ����ͨ��Ҫ�� EnemyDatabase ȡ����ԭ�ͣ��������ʾ ===
        Enemy e("Slime", enemyId, /*hp*/30, /*atk*/5, /*drops*/{ 101 });

        std::cout << "��������#" << e.getId() << " [" << e.getName() << "]\n";
        while (!e.isDead() && !player.isDead()) {
            e.takeDamage(player.atk());
            if (!e.isDead()) player.takeDamage(e.atk());
        }
        if (player.isDead()) {
            std::cout << "�㵹���ˡ���\n";
            break;
        }
        std::cout << "���ܵ���#" << e.getId() << "\n";
        totalLoot.insert(totalLoot.end(), e.drops().begin(), e.drops().end());
        // �Ƿ�ӷ������Ƴ��õ������н���/�� Room �����ṩ remove �ӿڴ���
    }

    return totalLoot;
}