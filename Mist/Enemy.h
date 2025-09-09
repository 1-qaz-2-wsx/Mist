#pragma once

#include "Entity.h"
#include "Stats.h"
#include <vector>

using json = nlohmann::json;

class Enemy : public Entity {
public:
    Stats baseStats; // 基础属性（从模板加载）
    int currentHp;   // 当前生命值（用于战斗中）

    // 战利品列表，存放的是物品ID
    std::vector<unsigned int> lootTable;

public:
    Enemy() = default;

    // 重置生命值为最大生命值
    void resetHealth() {
        currentHp = baseStats.maxHp;
    }

    // --- 重写(override)基类的虚函数 ---
    virtual void fromJson(const json& j) override;
    virtual void toJson(json& j) const override;

	// --- 战斗相关方法 ---
    void takeDamage(int damageAmount);
    bool isAlive() const;
};