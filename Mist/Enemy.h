#pragma once
#include <string>
#include "Item.h"

class Enemy {
public:
    std::string name;
    int health;
    int attack;
    int defense;
    Item loot;

    Enemy(std::string name, int health, int attack, int defense, Item loot);

    bool isAlive() const;
    void takeDamage(int damage);
};