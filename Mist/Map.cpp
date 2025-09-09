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
        std::cerr << "�޷��򿪵�ͼ�ļ�: " << filename << "\n";
        return false;
    }

    json j;
    file >> j;

    // �������������ID
    if (j.contains("map")) {
        for (auto& cell : j["map"]) {
            int x = cell["x"];
            int y = cell["y"];
            unsigned int id = cell["roomId"];
            positionToRoomId[{x, y}] = id;
        }
    }

    // ��ʼλ��
    player_ = { 0,0 };
    currentRoomId_ = roomAt(0, 0);

    return true;
}

bool Map::movePlayer(int x, int y) {
    auto it = positionToRoomId.find({ x,y });
    if (it == positionToRoomId.end()) {
        std::cout << "����ʲô��û�У��޷��ƶ���\n";
        return false;
    }

    player_ = { x,y };
    currentRoomId_ = it->second;

    auto room = db_.getRoom(currentRoomId_);
    if (room) {
        std::cout << "���뷿��: " << room->getName() << "\n";
    }
    return true;
}  //���潻��Game����������������ӣ�Mapֻ��������Ƿ��ƶ��ɹ����ź� Game�����źź󣬽��з���ļ���

unsigned int Map::getCurrentRoomId() const {
    return currentRoomId_;
}

unsigned int Map::roomAt(int x, int y) const {
    auto it = positionToRoomId.find({ x,y });
    if (it == positionToRoomId.end()) return 0; // 0 ��ʾû�з���
    return it->second;
}

void Map::printMap() const {
    std::cout << "���λ��: (" << player_.first << "," << player_.second << ")\n";
    std::cout << "��ǰ����ID: " << currentRoomId_ << "\n";

    for (auto& [pos, id] : positionToRoomId) {
        std::cout << "���� " << id << " ������ (" << pos.first << "," << pos.second << ")\n";
    }
}

Map::~Map() {}