#pragma once
#include "Entity.h"
#include <string>
#include <vector>

enum NPCType {
    HEALER,     // 华佗：治疗回血回蓝
    BEGGAR,     // 乞丐：给钱换道具
    GIVER       // 小黄人：对话直接给道具
};

class NPC : public Entity {
private:
    NPCType type;
    std::string dialogue;  // 默认对话
    unsigned int rewardItemId; // NPC给予的道具ID（乞丐/小黄人）
    int cost;              // 若需要付费（乞丐）

public:
    NPC();
    NPC(const std::string& name, unsigned int id, NPCType type);

    // 基础访问器
    NPCType getType() const;
    void setType(NPCType type);

    void setDialogue(const std::string& text);
    const std::string& getDialogue() const;

    void setRewardItem(unsigned int itemId);
    unsigned int getRewardItem() const;

    void setCost(int c);
    int getCost() const;

    // 玩家交互
    std::string interact(int& playerHp, int& playerMp, int& playerGold, std::vector<unsigned int>& playerInventory);

    // JSON 读写
    void toJson(json& j) const override;
    void fromJson(const json& j) override;
};
