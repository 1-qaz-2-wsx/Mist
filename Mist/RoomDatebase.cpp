#include "RoomDatabase.h"
#include "ItemDatabase.h"
#include "EnemyDatabase.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool RoomDatabase::load(const std::string& filename, const ItemDatabase* itemDB, const EnemyDatabase* enemyDB) {
    std::ifstream file(filename);
    if (!file.is_open()) { /* ... error handling ... */ return false; }

    json jsonData;
    try {
        file >> jsonData;
    }
    catch (const json::parse_error& e) { /* ... error handling ... */ return false; }

    roomTemplates_.clear();

    // 遍历JSON数组中的每个房间对象
    for (const auto& roomObject : jsonData) {
        Room newRoomTemplate;
        // 1. 先让 Room 自己加载基本信息 (ID, name, description, exits)
        newRoomTemplate.fromJson(roomObject);

        // 2. RoomDatabase 负责处理依赖项 (items 和 enemies)
        if (roomObject.contains("items")) {
            for (const auto& itemId : roomObject["items"]) {
                // 向 ItemDatabase 请求创建物品实例
                auto itemInstance = itemDB->createInstance(itemId.get<unsigned int>());
                if (itemInstance) {
                    newRoomTemplate.items.push_back(std::move(itemInstance));
                }
            }
        }

        if (roomObject.contains("enemies")) {
            for (const auto& enemyId : roomObject["enemies"]) {
                // 向 EnemyDatabase 请求创建敌人实例
                auto enemyInstance = enemyDB->createInstance(enemyId.get<unsigned int>());
                if (enemyInstance) {
                    newRoomTemplate.enemies.push_back(std::move(enemyInstance));
                }
            }
        }

        // 3. 将构建完成的模板存入数据库
        roomTemplates_[newRoomTemplate.getId()] = std::move(newRoomTemplate);
    }

    std::cout << "Loaded " << roomTemplates_.size() << " rooms from " << filename << std::endl;
    return true;
}

std::unique_ptr<Room> RoomDatabase::createInstance(unsigned int roomId) const {
    auto it = roomTemplates_.find(roomId);
    if (it != roomTemplates_.end()) {
        // 使用我们之前实现的拷贝构造函数来创建一个深拷贝的实例
        return std::make_unique<Room>(it->second);
    }
    return nullptr;
}

std::vector<unsigned int> RoomDatabase::getAllRoomIds() const {
    std::vector<unsigned int> ids;
    ids.reserve(roomTemplates_.size()); // 提前预留空间以提高效率
    for (const auto& pair : roomTemplates_) {
        ids.push_back(pair.first); // pair.first 是 map 中的 key (即房间ID)
    }
    return ids;
}