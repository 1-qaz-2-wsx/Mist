#pragma once
#include <vector>
#include "Room.h"

class Map {
private:
    std::vector<Room*> allRooms; // 用于管理所有房间的内存

public:
    Room* startRoom;

    Map();
    ~Map(); // 析构函数释放所有房间的内存

    void build();
};