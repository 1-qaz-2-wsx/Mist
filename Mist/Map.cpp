#include "Map.h"
#include "RoomDatabase.h"
#include "Room.h"
#include "json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

Map::Map(RoomDatabase& db) : db_(db) {}

bool Map::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开地图文件: " << filename << "\n";
        return false;
    }

    json j;
    file >> j;

    // 加载坐标→房间ID
    if (j.contains("map")) {
        for (auto& cell : j["map"]) {
            int x = cell["x"];
            int y = cell["y"];
            unsigned int id = cell["roomId"];
            positionToRoomId[{x, y}] = id;
        }
    }

    // 初始位置
    player_ = { 0,0 };
    currentRoomId_ = roomAt(0, 0);

    return true;
}

bool Map::movePlayer(int x, int y) {
    auto it = positionToRoomId.find({ x,y });
    if (it == positionToRoomId.end()) {
        std::cout << "这里什么都没有，无法移动！\n";
        return false;
    }

    player_ = { x,y };
    currentRoomId_ = it->second;

    auto room = db_.getRoom(currentRoomId_);
    if (room) {
        std::cout << "进入房间: " << room->getName() << "\n";
    }
    return true;
}  //后面交给Game，进行类与类的连接，Map只负责给出是否移动成功的信号 Game接收信号后，进行房间的加载

unsigned int Map::getCurrentRoomId() const {
    return currentRoomId_;
}

unsigned int Map::roomAt(int x, int y) const {
    auto it = positionToRoomId.find({ x,y });
    if (it == positionToRoomId.end()) return 0; // 0 表示没有房间
    return it->second;
}

void Map::printMap() const {
    std::cout << "玩家位置: (" << player_.first << "," << player_.second << ")\n";
    std::cout << "当前房间ID: " << currentRoomId_ << "\n";

    for (auto& [pos, id] : positionToRoomId) {
        std::cout << "房间 " << id << " 在坐标 (" << pos.first << "," << pos.second << ")\n";
    }
}

Map::~Map() {}