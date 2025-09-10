#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "Room.h"

class Player {
public:
    int health;
    int maxHealth;
    int attack;
    int defense;
    Room* currentRoom;
    std::vector<Item> inventory;

    Player(Room* startRoom);

    void move(const std::string& direction);
    void takeItem(const Item& item);
    void useItem(const std::string& itemName);
    void showStatus() const;
    void showInventory() const;
    bool isAlive() const;
    void takeDamage(int damage);
};