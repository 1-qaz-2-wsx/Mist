#include "Player.h"
#include <iostream>

Player::Player(Room* startRoom)
    : maxHealth(100), health(100), attack(10), defense(5), currentRoom(startRoom) {}

void Player::move(const std::string& direction) {
    auto it = currentRoom->exits.find(direction);
    if (it != currentRoom->exits.end()) {
        currentRoom = it->second;
        std::cout << "你移动到了 " << direction << ".\n";
    }
    else {
        std::cout << "那个方向没有路。\n";
    }
}

void Player::takeItem(const Item& item) {
    inventory.push_back(item);
    std::cout << "你将 " << item.name << " 放入了背包。\n";
}

void Player::useItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->name == itemName) {
            std::cout << "你使用了 " << it->name << "。 " << it->description << "\n";
            switch (it->effect) {
            case ItemEffect::HEAL:
                health += it->value;
                if (health > maxHealth) health = maxHealth;
                std::cout << "你的生命值恢复了 " << it->value << " 点。\n";
                break;
            case ItemEffect::ATTACK_BUFF:
                attack += it->value;
                std::cout << "你的攻击力永久提升了 " << it->value << " 点。\n";
                break;
            case ItemEffect::DEFENSE_BUFF:
                defense += it->value;
                std::cout << "你的防御力永久提升了 " << it->value << " 点。\n";
                break;
            }
            inventory.erase(it); // 使用后物品消失
            return;
        }
    }
    std::cout << "你的背包里没有叫做 '" << itemName << "' 的物品。\n";
}

void Player::showStatus() const {
    std::cout << "--- 玩家状态 ---\n";
    std::cout << "生命值: " << health << " / " << maxHealth << "\n";
    std::cout << "攻击力: " << attack << "\n";
    std::cout << "防御力: " << defense << "\n";
    std::cout << "------------------\n";
}

void Player::showInventory() const {
    std::cout << "--- 背包 ---\n";
    if (inventory.empty()) {
        std::cout << "你的背包是空的。\n";
    }
    else {
        for (const auto& item : inventory) {
            std::cout << "- " << item.name << " (" << item.description << ")\n";
        }
    }
    std::cout << "------------\n";
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}