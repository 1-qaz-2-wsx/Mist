#pragma once
#include <string>

// 定义物品类型
enum class ItemType {
    CONSUMABLE, // 消耗品
    WEAPON,     // 武器
    QUEST       // 任务物品
};


// 定义物品效果
enum class ItemEffect {
    HEAL,
    ATTACK_BUFF,
    DEFENSE_BUFF,
    WEAPON_PROFICIENCY_BUFF // 新增：武器熟练度提升
};

class Item {
public:
    std::string name;
    std::string description;
    ItemType type; 
    ItemEffect effect;
    int value; // 效果值，比如恢复的点数或增加的属性
    int attackBonus; // 武器的攻击力加成
    Item(std::string name = "", std::string description = "", ItemType type = ItemType::CONSUMABLE, ItemEffect effect = ItemEffect::HEAL, int value = 0, int attackBonus = 0);
};