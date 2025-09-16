#pragma once
#include <vector>
#include <map>      // 包含 map 头文件
#include <string>   // 包含 string 头文件
#include <memory> // 包含 <memory> 头文件
#include "Room.h"

class Map {
public:
    //std::vector<Room*> allRooms; // 用于管理所有房间的内存
    
    std::vector<std::shared_ptr<Room>> allRooms;

    // *** 新增：用于存储地图美术字的成员变量 ***
    std::string mapArt;
public:
    std::shared_ptr<Room> startRoom;
    std::shared_ptr<Room> hotelRoom;
    Map();
    //~Map(); // 析构函数释放所有房间的内存

    // 存储游戏中所有可能物品的原型
    std::map<std::string, Item> itemDatabase;

    void buildItemDatabase(); // 填充物品数据库
    Item getItemFromDatabase(const std::string& itemName) const; // 从数据库获取物品

    void build();
    // *** 新增：从文件加载地图的函数 ***
    void loadMapFromFile(const std::string& filePath);
    // *** 修改：将 printMap 变为非静态成员函数 ***
    void printMap() const;
};