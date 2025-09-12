#pragma once
#include <vector>
#include <map>      // 包含 map 头文件
#include <string>   // 包含 string 头文件
#include "Room.h"

class Map {
public:
    std::vector<Room*> allRooms; // 用于管理所有房间的内存

public:
    Room* startRoom;
    Room* hotelRoom;
    Map();
    ~Map(); // 析构函数释放所有房间的内存

    // 存储游戏中所有可能物品的原型
    std::map<std::string, Item> itemDatabase;

    void buildItemDatabase(); // 填充物品数据库
    Item getItemFromDatabase(const std::string& itemName) const; // 从数据库获取物品

    void build();
    static void printMap();
};