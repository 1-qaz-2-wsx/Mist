#include "Player.h"
#include <iostream>
#include <algorithm> // 需要包含 akgorithm 来使用 std::find_if
#include "Map.h"
Player::Player(std::shared_ptr<Room> startRoom)
    : currentRoom(startRoom),
    name("玩家"),
	maxHealth(150), //修改初始最大生命值为150
    health(150),
    attack(10),
    defense(5),
    agility(5),
    intelligence(5),
    maxStamina(8),
    stamina(8),
    money(100),
    weaponProficiency(0),
    isUltimateCharged(false)
{}

bool Player::move(const std::string& direction, Map& gameMap) {
    if (stamina <= 0) {
        std::cout << "你太累了，无法移动。\n";
        std::cout << "是否花费 5 G 传送至饭店恢复体力? (y/n)\n> ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            if (money >= 5) {
                money -= 5;
                currentRoom = gameMap.hotelRoom; // 传送到饭店
                std::cout << "你支付了 5 G，闪来将你传送到了【金碧辉煌的饭店】门口。\n";
            }
            else {
                std::cout << "你的金钱不足，无法使用传送服务。\n";
            }
            std::cout << "\n(按回车键继续...)\n";
            std::cin.get(); // 等待玩家按回车
        }
        return false; // 移动失败
    }

    auto it = currentRoom->exits.find(direction);
    if (it != currentRoom->exits.end()) {
        //currentRoom = it->second;
        //stamina--; // 移动消耗体力
        //std::cout << "你移动到了 " << currentRoom->getName() << "，消耗了1点体力。\n";
        //return true; // 移动成功
        // 使用 lock() 从 weak_ptr 获取 shared_ptr
        if (auto nextRoom = it->second.lock()) {
            currentRoom = nextRoom;
            stamina--;
            std::cout << "你移动到了 " << currentRoom->getName() << "，消耗了1点体力。\n";
            return true;
        }
    }

    else {
        std::cout << "那个方向没有路。\n";
        return false; // 移动失败

    }
}


void Player::takeItem(const Item& item) {
    inventory.push_back(item);
    std::cout << "你将 " << item.name << " 放入了背包。\n";
}
void Player::equipWeapon(const std::string& weaponName) {
    for (const auto& item : inventory) {
        if (item.name == weaponName && item.type == ItemType::WEAPON) {
            equippedWeapon = item;
            std::cout << "你装备了 " << weaponName << "。\n";
            return;
        }
    }
    std::cout << "你没有名为 '" << weaponName << "' 的武器。\n";
}
void Player::useItem(const std::string& itemName) {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->name == itemName) {
            if (it->type == ItemType::WEAPON) {
                equipWeapon(itemName); // 如果是武器，则装备它
                return;
            }

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
            case ItemEffect::WEAPON_PROFICIENCY_BUFF:
                weaponProficiency += it->value;
                std::cout << "你的武器熟练度提升了 " << it->value << " 点！\n";
                break;
            case ItemEffect::AGILITY_BUFF:
                agility += it->value;
                std::cout << "你的敏捷提升了 " << it->value << " 点！\n";
                break;
            case ItemEffect::INTELLIGENCE_BUFF:
                intelligence += it->value;
                std::cout << "你感觉头脑清明，智力永久提升了 " << it->value << " 点！\n";
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
    std::cout << "体力:   " << stamina << " / " << maxStamina << "\n";
    std::cout << "攻击力: " << attack << "\n";
    std::cout << "防御力: " << defense << "\n";
    std::cout << "敏捷:   " << agility << "\n";
    std::cout << "智力:   " << intelligence << "\n";
    std::cout << "武器熟练度: " << weaponProficiency << "\n";
    std::cout << "金钱:   " << money << " G\n";
    if (!equippedWeapon.name.empty()) {
        std::cout << "装备中: " << equippedWeapon.name << "\n";
    }
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

int Player::calculateAttack() {
    return attack + equippedWeapon.attackBonus + weaponProficiency;
}