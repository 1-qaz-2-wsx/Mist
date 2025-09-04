#include "Room.h"
#include <algorithm>
#include <fstream>
#include <string>
//���캯��
Room::Room() : Entity(), type(DEFAULT), des("A nondescript room.") {}
Room::Room(const std::string& name, unsigned int id, RoomType type, const std::string& des)
	: Entity(name, id), type(type), des(des) {}

//������
RoomType Room::getType() const {
	return type;
}

const std::string& Room::getDes() const {
	return des;
}

//�޸���
void Room::setType(RoomType type) {
	this->type = type;
}

void Room::setDes(const std::string& des) {
	this->des = des;
}

//�������
void Room::addExit(DIRECTION dir, unsigned int roomId) {
	exits[dir] = roomId;
}
const std::map<DIRECTION, unsigned int>& Room::getExits() const {
	return exits;
}

bool Room::removeExit(DIRECTION dir) {
	return exits.erase(dir) > 0;
}
bool Room::hasExit(DIRECTION dir) const {
	return exits.find(dir) != exits.end();
}
unsigned int Room::getExit(DIRECTION dir) const {
	auto it = exits.find(dir);
	if (it != exits.end()) {
		return it->second;
	}
	return 0; //���޸÷��򷵻�0
}

//��Ʒ���
void Room::addItem(unsigned int itemId) {
	items.push_back(itemId);
}
bool Room::removeItem(unsigned int itemId) {
	auto it = std::find(items.begin(), items.end(), itemId);
	if (it != items.end()) {
		items.erase(it);
		return true;
	}
	return false;
}
const std::vector<unsigned int>& Room::getItems() const {
	return items;
}
bool Room::hasItem(unsigned int itemId) const {
	return std::find(items.begin(), items.end(), itemId) != items.end();
}

//�������
void Room::addEnemy(unsigned int enemyId) {
	enemys.push_back(enemyId);
}
bool Room::removeEnemy(unsigned int enemyId) {
	auto it = std::find(enemys.begin(), enemys.end(), enemyId);
	if (it != enemys.end()) {
		enemys.erase(it);
		return true;
	}
	return false;
}
const std::vector<unsigned int>& Room::getEnemys() const {
	return enemys;
}
bool Room::hasEnemy(unsigned int enemyId) const {
	return std::find(enemys.begin(), enemys.end(), enemyId) != enemys.end();
}

//NPC���
void Room::addNPC(unsigned int npcId) {
	npcs.push_back(npcId);
}
bool Room::removeNPC(unsigned int npcId) {
	auto it = std::find(npcs.begin(), npcs.end(), npcId);
	if (it != npcs.end()) {
		npcs.erase(it);
		return true;
	}
	return false;
}
const std::vector<unsigned int>& Room::getNPCs() const {
	return npcs;
}
bool Room::hasNPC(unsigned int npcId) const {
	return std::find(npcs.begin(), npcs.end(), npcId) != npcs.end();
}

Room::~Room() {}


// JSON ��д
void Room::toJson(json& j) const {
	Entity::toJson(j); //���û����toJson����дname ��id
	// ������������
	j["type"] = static_cast<int>(type);
	j["description"] = des;

	// ����
	j["exits"] = json::object();
	for (const auto& [dir, roomId] : exits) {
		j["exits"][std::to_string(static_cast<int>(dir))] = roomId;
	}

	// ��Ʒ
	j["items"] = items;

	// ����
	j["enemys"] = enemys;

	// NPC
	j["npcs"] = npcs;
}

void Room::fromJson(const json& j) {
	Entity::fromJson(j); //���û����fromJson���ȶ�name ��id

	// ������������
	if (j.contains("type") && j["type"].is_number_integer()) {
		type = static_cast<RoomType>(j["type"].get<int>());
	}
	if (j.contains("description") && j["description"].is_string()) {
		des = j["description"].get<std::string>();
	}

	// ����
	exits.clear(); //��������г��ڣ��������ݲ���
	if (j.contains("exits") && j["exits"].is_object()) {
		for (auto& [key, value] : j["exits"].items()) {
			if (value.is_number_unsigned()) {
				DIRECTION dir = static_cast<DIRECTION>(std::stoi(key));
				exits[dir] = value.get<unsigned int>();
			}
		}
	}

	// ��Ʒ
	items.clear();
	if (j.contains("items") && j["items"].is_array()) {
		for (const auto& item : j["items"]) {
			if (item.is_number_unsigned()) {
				items.push_back(item.get<unsigned int>());
			}
		}
	}

	// ����
	enemys.clear();
	if (j.contains("enemys") && j["enemys"].is_array()) {
		for (const auto& enemy : j["enemys"]) {
			if (enemy.is_number_unsigned()) {
				enemys.push_back(enemy.get<unsigned int>());
			}
		}
	}

	// NPC
	npcs.clear();
	if (j.contains("npcs") && j["npcs"].is_array()) {
		for (const auto& npc : j["npcs"]) {
			if (npc.is_number_unsigned()) {
				npcs.push_back(npc.get<unsigned int>());
			}
		}
	}
}
