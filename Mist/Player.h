#pragma once

#include "Entity.h"
#include "Stats.h"
#include <vector>
#include <memory>

// 前向声明
class Item;
class Game;

class Player : public Entity {
public:
    Stats stats;
    int currentHp;

private:
    unsigned int currentRoomId_;
    std::vector<std::unique_ptr<Item>> inventory_;
    Game* mediator_ = nullptr; // 指向中介者(Game)的非拥有指针

public:
    Player();

    // --- 核心方法 ---

    /**
     * @brief 向背包中添加一个物品
     * @param item 一个指向物品实例的 unique_ptr
     */
    void addItemToInventory(std::unique_ptr<Item> item);

    /**
     * @brief 显示玩家的背包
     */
    void displayInventory() const;

    /**
     * @brief 受到伤害
     * @param damageAmount 受到的伤害值
     */
    void takeDamage(int damageAmount);

    /**
     * @brief 检查玩家是否还存活
     * @return true 如果 currentHp > 0
     */
    bool isAlive() const;

    // --- Getters & Setters ---

    void setMediator(Game* game) { mediator_ = game; }
    void setCurrentRoomId(unsigned int id) { currentRoomId_ = id; }
    unsigned int getCurrentRoomId() const { return currentRoomId_; }
};