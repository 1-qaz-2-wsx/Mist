#pragma once
#include <string>
#include <map>

// 前向声明以避免循环引用
class Item;
class Enemy;
class NPC;

class Room {
public:
    std::string description;
    std::map<std::string, Room*> exits;

    Enemy* enemy;
    NPC* npc;
    Item* item;

    Room(std::string description);
    ~Room(); // 需要析构函数来释放动态分配的内存

    void look() const;
};