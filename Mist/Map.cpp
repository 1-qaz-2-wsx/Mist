#include "Map.h"
#include "Room.h"
#include "RoomDatabase.h"

void Map::buildFromDatabase(RoomDatabase* roomDB) {
    // 1. 清空旧的地图数据
    rooms_.clear();

    // 2. 从数据库获取所有房间的ID
    std::vector<unsigned int> allIds = roomDB->getAllRoomIds();

    // 3. 为每一个ID，从数据库创建一个房间实例，并存入地图中
    for (unsigned int id : allIds) {
        // roomDB->createInstance 会返回一个包含完整敌人和物品的房间深拷贝
        rooms_[id] = roomDB->createInstance(id);
    }

    // 你可以在这里添加逻辑来从某个地方读取起始房间ID，
    // 但目前我们使用默认值 1
}

Room* Map::getRoom(unsigned int roomId) {
    auto it = rooms_.find(roomId);
    if (it != rooms_.end()) {
        // .get() 方法从 unique_ptr 返回一个裸指针
        return it->second.get();
    }
    return nullptr;
}

unsigned int Map::getStartingRoomId() const {
    return startingRoomId_;
}