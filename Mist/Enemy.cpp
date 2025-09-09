#include "Enemy.h"
#include <iostream>
void Enemy::fromJson(const json& j) {
    // 1. 调用基类函数加载 id 和 name
    Entity::fromJson(j);

    // 2. 加载 Stats
    if (j.contains("stats")) {
        baseStats.fromJson(j.at("stats"));
    }

    // 3. 加载战利品列表
    if (j.contains("lootTable")) {
        lootTable = j.at("lootTable").get<std::vector<unsigned int>>();
    }

    // 4. 初始化当前生命值
    resetHealth();
}

void Enemy::toJson(json& j) const {
    // 1. 调用基类函数序列化 id 和 name
    Entity::toJson(j);

    // 2. 序列化 Stats
    json statsJson;
    baseStats.toJson(statsJson);
    j["stats"] = statsJson;

    // 3. 序列化战利品列表
    j["lootTable"] = lootTable;
}


void Enemy::takeDamage(int damageAmount) {
    int actualDamage = damageAmount - baseStats.defense;
    if (actualDamage < 1) actualDamage = 1;

    currentHp -= actualDamage;
    std::cout << getName() << " 受到了 " << actualDamage << " 点伤害。" << std::endl;

    if (!isAlive()) {
        std::cout << getName() << " 被击败了！" << std::endl;
    }
}

bool Enemy::isAlive() const {
    return currentHp > 0;
}