#pragma once

#include "Entity.h"
#include "Stats.h"
#include <vector>

using json = nlohmann::json;

class Enemy : public Entity {
public:
    Stats baseStats; // �������ԣ���ģ����أ�
    int currentHp;   // ��ǰ����ֵ������ս���У�

    // ս��Ʒ�б���ŵ�����ƷID
    std::vector<unsigned int> lootTable;

public:
    Enemy() = default;

    // ��������ֵΪ�������ֵ
    void resetHealth() {
        currentHp = baseStats.maxHp;
    }

    // --- ��д(override)������麯�� ---
    virtual void fromJson(const json& j) override;
    virtual void toJson(json& j) const override;

	// --- ս����ط��� ---
    void takeDamage(int damageAmount);
    bool isAlive() const;
};