#include "Room.h"
#include "Enemy.h" // 在.cpp中包含完整定义
#include "Item.h"  // 在.cpp中包含完整定义
#include <iostream>

// 实现拷贝构造函数，用于创建房间的“实例”
Room::Room(const Room& other) : Entity(other) {
    this->description = other.description;
    this->exits = other.exits;
    // 关键：为 unique_ptr 创建深拷贝
    for (const auto& enemy : other.enemies) {
        this->enemies.push_back(std::make_unique<Enemy>(*enemy));
    }
    for (const auto& item : other.items) {
        this->items.push_back(std::make_unique<Item>(*item));
    }
}

// 实现拷贝赋值运算符
Room& Room::operator=(const Room& other) {
    if (this == &other) {
        return *this;
    }
    Entity::operator=(other);
    this->description = other.description;
    this->exits = other.exits;
    this->enemies.clear();
    this->items.clear();
    for (const auto& enemy : other.enemies) {
        this->enemies.push_back(std::make_unique<Enemy>(*enemy));
    }
    for (const auto& item : other.items) {
        this->items.push_back(std::make_unique<Item>(*item));
    }
    return *this;
}


void Room::display() const {
    // 1. 显示房间名和描述
    std::cout << "--- " << name_ << " ---" << std::endl;
    std::cout << description << std::endl;

    // 2. 显示地上的物品
    if (!items.empty()) {
        std::cout << "\n[地上有]: ";
        for (const auto& item : items) {
            std::cout << item->getName() << " ";
        }
        std::cout << std::endl;
    }

    // 3. 显示房间里的敌人
    if (!enemies.empty()) {
        std::cout << "\n[敌人]: ";
        for (const auto& enemy : enemies) {
            std::cout << enemy->getName() << " ";
        }
        std::cout << std::endl;
    }

    // 4. 显示出口
    std::cout << "\n[出口]: ";
    if (exits.empty()) {
        std::cout << "无";
    }
    else {
        for (const auto& pair : exits) {
            std::cout << pair.first << " "; // pair.first 是方向
        }
    }
    std::cout << std::endl;
}

int Room::getExit(const std::string& direction) const {
    auto it = exits.find(direction);
    if (it != exits.end()) {
        return it->second; // it->second 是房间ID
    }
    return -1; // -1 表示未找到
}

void Room::fromJson(const json& j) {
    // Room::fromJson 只负责加载它自己的直接数据
    Entity::fromJson(j);
    description = j.at("description").get<std::string>();

    // 加载出口信息
    if (j.contains("exits")) {
        exits = j.at("exits").get<std::map<std::string, unsigned int>>();
    }
}

std::unique_ptr<Item> Room::takeItem(const std::string& itemName) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getName() == itemName) {
            std::unique_ptr<Item> foundItem = std::move(*it);
            items.erase(it);
            return foundItem;
        }
    }
    return nullptr;
}