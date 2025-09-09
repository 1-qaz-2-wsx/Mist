#include "Player.h"
#include "Item.h" // 需要包含完整定义
#include "Game.h" // 用于通知中介者
#include <iostream>

Player::Player() {
    // 初始化玩家的默认属性
    // 以后这里可以从"player_save.json"之类的地方加载
    name_ = "冒险者"; // 继承自 Entity
    id_ = 0;           // 继承自 Entity, 玩家ID默认是0

    stats.maxHp = 100;
    stats.attack = 10;
    stats.defense = 5;

    currentHp = stats.maxHp;
    currentRoomId_ = 1; // 默认初始房间ID，Game::initialize会覆盖它
}

void Player::addItemToInventory(std::unique_ptr<Item> item) {
    if (item) {
        std::cout << "你将 [" << item->getName() << "] 放入了背包。" << std::endl;
        inventory_.push_back(std::move(item));
    }
}

void Player::displayInventory() const {
    std::cout << "--- 你的背包 ---" << std::endl;
    if (inventory_.empty()) {
        std::cout << "空空如也..." << std::endl;
    }
    else {
        for (const auto& item : inventory_) {
            std::cout << "- " << item->getName() << " (" << item->description << ")" << std::endl;
        }
    }
    std::cout << "----------------" << std::endl;
}

void Player::takeDamage(int damageAmount) {
    // 实际伤害 = 攻击力 - 防御力 (可以设计更复杂的公式)
    int actualDamage = damageAmount - stats.defense;
    if (actualDamage < 1) {
        actualDamage = 1; // 保证至少受到1点伤害
    }

    currentHp -= actualDamage;
    std::cout << "你受到了 " << actualDamage << " 点伤害！" << std::endl;

    if (!isAlive()) {
        std::cout << "你的生命值降到了0..." << std::endl;
        // [中介者模式]
        // 玩家死亡，它不自己处理游戏结束逻辑，而是通知中介者
        // if (mediator_) {
        //     mediator_->notifyPlayerDied(); // 我们之后可以在Game中添加这个函数
        // }
    }
}

bool Player::isAlive() const {
    return currentHp > 0;
}