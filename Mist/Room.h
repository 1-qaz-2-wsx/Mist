#pragma once

#include "Entity.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

// 前向声明，避免在头文件中引入完整的 Item.h 和 Enemy.h，降低耦合度
class Item;
class Enemy;

using json = nlohmann::json;

class Room : public Entity {
public:
    std::string description;

    // 出口列表: 键是方向(string)，值是目标房间的ID(unsigned int)
    std::map<std::string, unsigned int> exits;

    // 房间内的敌人列表。使用智能指针，因为每个敌人都是一个独立实例
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 房间地上的物品列表
    std::vector<std::unique_ptr<Item>> items;

public:
    Room() = default;

    // 由于 unique_ptr 的存在，Room 类是不可拷贝的。
    // 我们需要自己实现一个“深拷贝”或“克隆”方法来创建实例。
    Room(const Room& other); // 拷贝构造函数
    Room& operator=(const Room& other); // 拷贝赋值运算符


    // --- 核心方法 ---

    /**
     * @brief 显示房间的所有信息（描述、出口、物品、敌人）
     */
    void display() const;

    /**
     * @brief 根据方向获取出口指向的房间ID
     * @param direction "north", "south", etc.
     * @return 房间ID，如果该方向没有出口则返回 -1
     */
    int getExit(const std::string& direction) const;

    // --- 重写(override)基类的虚函数 ---
    virtual void fromJson(const json& j) override;
    // toJson 我们暂时用不到，可以先不实现
    // virtual void toJson(json& j) const override;

    std::unique_ptr<Item> takeItem(const std::string& itemName);

};