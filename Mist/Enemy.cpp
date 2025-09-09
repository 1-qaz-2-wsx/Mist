#include "Enemy.h"

void Enemy::fromJson(const json& j) {
    // 1. ���û��ຯ������ id �� name
    Entity::fromJson(j);

    // 2. ���� Stats
    if (j.contains("stats")) {
        baseStats.fromJson(j.at("stats"));
    }

    // 3. ����ս��Ʒ�б�
    if (j.contains("lootTable")) {
        lootTable = j.at("lootTable").get<std::vector<unsigned int>>();
    }

    // 4. ��ʼ����ǰ����ֵ
    resetHealth();
}

void Enemy::toJson(json& j) const {
    // 1. ���û��ຯ�����л� id �� name
    Entity::toJson(j);

    // 2. ���л� Stats
    json statsJson;
    baseStats.toJson(statsJson);
    j["stats"] = statsJson;

    // 3. ���л�ս��Ʒ�б�
    j["lootTable"] = lootTable;
}