#pragma once

#include <unordered_map>
#include <memory>

// 前向声明
class Room;
class RoomDatabase;

class Map {
private:
    // 地图持有游戏世界中所有房间的“实例”的所有权
    // 键是房间ID，值是指向Room实例的智能指针
    std::unordered_map<unsigned int, std::unique_ptr<Room>> rooms_;

    unsigned int startingRoomId_ = 1; // 玩家默认出生房间ID

public:
    Map() = default;

    /**
     * @brief 根据RoomDatabase中的模板，构建整个地图的房间实例
     * @param roomDB 指向房间数据库的指针
     */
    void buildFromDatabase(RoomDatabase* roomDB);

    /**
     * @brief 通过ID获取一个指向房间的指针
     * @param roomId 要获取的房间ID
     * @return 指向Room的裸指针，如果找不到则为nullptr
     */
    Room* getRoom(unsigned int roomId);

    /**
     * @brief 获取玩家的起始房间ID
     */
    unsigned int getStartingRoomId() const;
};