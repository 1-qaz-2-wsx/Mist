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

//��
bool RoomDatabase::addRoom(const std::shared_ptr<Room>& room) {
	if (rooms.find(room->getId()) != rooms.end()) {
		return false; // ID�Ѵ���
	}
	rooms[room->getId()] = room;
	return true;
}

//ɾ
bool RoomDatabase::removeRoom(unsigned int roomId) {
	auto it = rooms.find(roomId);
	if (it == rooms.end()) {
		return false; // ID������
	}
	rooms.erase(it);
	return true;
}

//�� 
std::shared_ptr<Room> RoomDatabase::getRoom(unsigned int roomId) const {
	auto it = rooms.find(roomId);
	if (it == rooms.end()) {
		return nullptr; // ID������
	}
	return it->second;
}

//���
bool RoomDatabase::hasRoom(unsigned int roomId) const {
	return rooms.find(roomId) != rooms.end();
}

const std::map<unsigned int, std::shared_ptr<Room>>& RoomDatabase::getAllRooms() const {
	return rooms;
}

// JSON ��д
bool RoomDatabase::loadFromFile(const std::string& filename) {
	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		return false; // �ļ���ʧ��
	}

	json j;
	inFile >> j;
	inFile.close();

	if (!j.is_array()) {
		return false; // JSON ��ʽ����
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
		return false; // �ļ���ʧ��
	}

	json j = json::array();  //���� JSON ���飬����������ʽ�洢����
	for (const auto& [id, room] : rooms) {
		json roomJson;
		room->toJson(roomJson); //���� Room �� toJson ������roomJson �洢�÷���� JSON ����
		j.push_back(roomJson);
	}

	outFile << j.dump(4); // �������������4���ո�
	outFile.close();
	return true;
}


