#include "RoomDatabase.h"
#include "Room.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

RoomDatabase::RoomDatabase() {}
RoomDatabase::RoomDatabase(const std::string& filename) {
	loadFromFile(filename);
}

RoomDatabase::~RoomDatabase() {}

//增
bool RoomDatabase::addRoom(const std::shared_ptr<Room>& room) {
	if (rooms.find(room->getId()) != rooms.end()) {
		return false; // ID已存在
	}
	rooms[room->getId()] = room;
	return true;
}

//删
bool RoomDatabase::removeRoom(unsigned int roomId) {
	auto it = rooms.find(roomId);
	if (it == rooms.end()) {
		return false; // ID不存在
	}
	rooms.erase(it);
	return true;
}

//查 
std::shared_ptr<Room> RoomDatabase::getRoom(unsigned int roomId) const {
	auto it = rooms.find(roomId);
	if (it == rooms.end()) {
		return nullptr; // ID不存在
	}
	return it->second;
}

//检查
bool RoomDatabase::hasRoom(unsigned int roomId) const {
	return rooms.find(roomId) != rooms.end();
}

const std::map<unsigned int, std::shared_ptr<Room>>& RoomDatabase::getAllRooms() const {
	return rooms;
}

// JSON 读写
bool RoomDatabase::loadFromFile(const std::string& filename) {
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		return false; // 文件打开失败
	}

	json j;
	inFile >> j;
	inFile.close();

	if (!j.is_array()) {
		return false; // JSON 格式错误
	}

	rooms.clear();
	for (const auto& item : j) {
		auto room = std::make_shared<Room>();
		room->fromJson(item);
		rooms[room->getId()] = room;
	}
	return true;
}

bool RoomDatabase::saveToFile(const std::string& filename) const {
	std::ofstream outFile(filename);
	if (!outFile.is_open()) {
		return false; // 文件打开失败
	}

	json j = json::array();  //创建 JSON 数组，即以数组形式存储房间
	for (const auto& [id, room] : rooms) {
		json roomJson;
		room->toJson(roomJson); //调用 Room 的 toJson 方法，roomJson 存储该房间的 JSON 数据
		j.push_back(roomJson);
	}

	outFile << j.dump(4); // 美化输出，缩进4个空格
	outFile.close();
	return true;
}


