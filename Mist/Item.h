#pragma once
#include <string>

// 定义物品类型，用于决定物品效果
enum class ItemEffect {
    HEAL,
    ATTACK_BUFF,
    DEFENSE_BUFF
};

class Item {
public:
    std::string name;
    std::string description;
    ItemEffect effect;
    int value; // 效果值，比如恢复的点数或增加的属性

    Item(std::string name = "", std::string description = "", ItemEffect effect = ItemEffect::HEAL, int value = 0);
};