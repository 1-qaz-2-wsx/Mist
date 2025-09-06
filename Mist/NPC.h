#pragma once
#include "Entity.h"
#include <string>
#include <vector>

enum NPCType {
    HEALER,     // ��٢�����ƻ�Ѫ����
    BEGGAR,     // ��ؤ����Ǯ������
    GIVER       // С���ˣ��Ի�ֱ�Ӹ�����
};

class NPC : public Entity {
private:
    NPCType type;
    std::string dialogue;  // Ĭ�϶Ի�
    unsigned int rewardItemId; // NPC����ĵ���ID����ؤ/С���ˣ�
    int cost;              // ����Ҫ���ѣ���ؤ��

public:
    NPC();
    NPC(const std::string& name, unsigned int id, NPCType type);

    // ����������
    NPCType getType() const;
    void setType(NPCType type);

    void setDialogue(const std::string& text);
    const std::string& getDialogue() const;

    void setRewardItem(unsigned int itemId);
    unsigned int getRewardItem() const;

    void setCost(int c);
    int getCost() const;

    // ��ҽ���
    std::string interact(int& playerHp, int& playerMp, int& playerGold, std::vector<unsigned int>& playerInventory);

    // JSON ��д
    void toJson(json& j) const override;
    void fromJson(const json& j) override;
};
