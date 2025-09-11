#pragma once
#include <string>
#include <vector>
#include <map> // 用于存储武器
#include "Item.h"
#include "Room.h"

class Map;

class Player {
public:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;

    int agility;        // 敏捷
    int intelligence;   // 智力
    int stamina;        // 体力
    int maxStamina;     // 最大体力
    int money;          // 金钱
    int weaponProficiency; // 武器熟练度
    Item equippedWeapon; // 当前装备的武器

    bool isUltimateCharged; // 是否已蓄力准备放大招

    Room* currentRoom;
    std::vector<Item> inventory;

    Player(Room* startRoom);

    void move(const std::string& direction, Map& gameMap);
    void takeItem(const Item& item);
    void useItem(const std::string& itemName);
    void equipWeapon(const std::string& weaponName); // 装备武器
    void showStatus() const;
    void showInventory() const;
    bool isAlive() const;
    void takeDamage(int damage);
    int calculateAttack(); // 计算玩家的总攻击力
};