#include "NPC.h"
#include <iostream>

NPC::NPC() : Entity(), type(HEALER), rewardItemId(0), cost(0) {}
NPC::NPC(const std::string& name, unsigned int id, NPCType type)
    : Entity(name, id), type(type), rewardItemId(0), cost(0) {}

NPCType NPC::getType() const { return type; }
void NPC::setType(NPCType type) { this->type = type; }

void NPC::setDialogue(const std::string& text) { dialogue = text; }
const std::string& NPC::getDialogue() const { return dialogue; }

void NPC::setRewardItem(unsigned int itemId) { rewardItemId = itemId; }
unsigned int NPC::getRewardItem() const { return rewardItemId; }

void NPC::setCost(int c) { cost = c; }
int NPC::getCost() const { return cost; }

// 交互逻辑
std::string NPC::interact(int& playerHp, int& playerMp, int& playerGold, std::vector<unsigned int>& playerInventory) {
    switch (type) {
    case HEALER:
        playerHp = 100; // 直接拉满
        playerMp = 100;
        return name + " 治疗了你，你恢复了所有生命值和法力值！";

    case BEGGAR:
        if (playerGold >= cost) {
            playerGold -= cost;
            playerInventory.push_back(rewardItemId);
            return name + " 接过了你的钱，递给你一个物品(ID: " + std::to_string(rewardItemId) + ")";
        }
        else {
            return name + " 摇摇头：你没钱，不能帮助你。";
        }

    case GIVER:
        playerInventory.push_back(rewardItemId);
        return name + " 开心地和你聊天，然后送了你一个物品(ID: " + std::to_string(rewardItemId) + ")";

    default:
        return name + " 没有任何回应。";
    }
}

// JSON 读写
void NPC::toJson(json& j) const {
    Entity::toJson(j);
    j["type"] = static_cast<int>(type);
    j["dialogue"] = dialogue;
    j["rewardItemId"] = rewardItemId;
    j["cost"] = cost;
}

void NPC::fromJson(const json& j) {
    Entity::fromJson(j);
    if (j.contains("type")) type = static_cast<NPCType>(j["type"].get<int>());
    if (j.contains("dialogue")) dialogue = j["dialogue"].get<std::string>();
    if (j.contains("rewardItemId")) rewardItemId = j["rewardItemId"].get<unsigned int>();
    if (j.contains("cost")) cost = j["cost"].get<int>();
}
