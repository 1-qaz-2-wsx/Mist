#include "RoomDatabase.h"
#include "WorldData.h"  // [新增] 包含我们的新数据文件
#include <iostream>

// [重大修改] load 函数的全新实现
bool RoomDatabase::load() {
    std::cout << "Loading world data from source code..." << std::endl;

    // 从 WorldData.h 获取所有房间的模板
    std::vector<Room> room_templates_vec = createWorldRooms();

    // 清空并填充我们的 roomTemplates_ 哈希表
    roomTemplates_.clear();
    for (auto& room : room_templates_vec) {
        roomTemplates_[room.getId()] = std::move(room);
    }

    std::cout << "Loaded " << roomTemplates_.size() << " rooms from source." << std::endl;
    return true;
}

std::unique_ptr<Room> RoomDatabase::createInstance(unsigned int roomId) const {
    auto it = roomTemplates_.find(roomId);
    if (it != roomTemplates_.end()) {
        return std::make_unique<Room>(it->second);
    }
    return nullptr;
}

std::vector<unsigned int> RoomDatabase::getAllRoomIds() const {
    std::vector<unsigned int> ids;
    ids.reserve(roomTemplates_.size());
    for (const auto& pair : roomTemplates_) {
        ids.push_back(pair.first);
    }
    return ids;
}