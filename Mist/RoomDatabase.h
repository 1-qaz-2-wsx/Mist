#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Room.h"

// [修改] 不再需要前向声明 ItemDatabase 和 EnemyDatabase
// class ItemDatabase;
// class EnemyDatabase;

class RoomDatabase {
public:
    // [修改] load 函数不再需要任何参数
    bool load();

    std::unique_ptr<Room> createInstance(unsigned int roomId) const;
    std::vector<unsigned int> getAllRoomIds() const;

private:
    std::unordered_map<unsigned int, Room> roomTemplates_;
};