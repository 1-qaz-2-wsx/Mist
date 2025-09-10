#include "Item.h"

// 确保函数签名与.h文件中的声明一致
Item::Item(std::string name, std::string description, ItemType type, ItemEffect effect, int value, int attackBonus)
    : name(name), description(description), type(type), effect(effect), value(value), attackBonus(attackBonus)
{
  
}