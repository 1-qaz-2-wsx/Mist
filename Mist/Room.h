#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>

// 前向声明以避免循环引用
class Item;
class Enemy;
class NPC;
class Room;

class Room {
public:
	//unsigned int id_; // 房间唯一标识符
	std::string name;
    std::string description;
    //std::map<std::string, Room*> exits;
    std::map<std::string, std::weak_ptr<Room>> exits;
    std::vector<Item*> items; // 存储多个物品

    Enemy* enemy;
    NPC* npc;
    Item* item;

    Room(std::string name, std::string description);
    ~Room(); // 需要析构函数来释放动态分配的内存

    void look() const;
    void takeItem(Item* item);// 这个函数名其实更适合叫 addItem
    // 根据名称移除并返回物品指针，如果找不到则返回 nullptr
    Item* removeItem(const std::string& itemName);

    std::string getName();

};