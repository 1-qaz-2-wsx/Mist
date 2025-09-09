#include "Room.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
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

std::vector<unsigned int> Room::dropLoot() const {
	// �ⲿ�������ݵ���ID��ȡ������ƷID�б�
	std::vector<unsigned int> loot;

	// ���ݷ��������ж��ǵ��˵��䡢NPC���䣬�����̵���Ʒ
	if (type == RoomType::FIGHT) {
		// �������������ս����FIGHT�������˵�����Ʒ
		loot = dropLootFromEnemies();
	}
	else if (type == RoomType::ENCOUNTER) {
		// �������������ż����ENCOUNTER����NPC������Ʒ
		loot = dropLootFromNPCs();
	}
	else if (type == RoomType::STORE) {
		// �̵����͵ķ���ͨ����������Ʒ��������ҿ��Թ���
		loot = dropLootFromStore();
	}

	return loot;
}

std::vector<unsigned int> Room::dropLootFromEnemies() const {
	std::vector<unsigned int> loot;
	for (auto& enemyId : enemys) {
		auto lootForEnemy = getLootFromEnemy(enemyId);
		loot.insert(loot.end(), lootForEnemy.begin(), lootForEnemy.end());
	}
	return loot;
}

std::vector<unsigned int> Room::dropLootFromNPCs() const {
	std::vector<unsigned int> loot;
	for (auto& npcId : npcs) {
		auto lootForNPC = getLootFromNPC(npcId);
		loot.insert(loot.end(), lootForNPC.begin(), lootForNPC.end());
	}
	return loot;
}


//����������������������������������
std::vector<unsigned int> Room::dropLootFromStore() const {
	std::vector<unsigned int> loot;
	// �̵����Ʒ������Ԥ��õĿ�棬�����ǿ����ƷID
	loot.push_back(4001); // �����̵���IDΪ4001����Ʒ����   //������Ը���ʵ���������
	return loot;
}
//����������������������������������



// ģ����ݵ���ID��ȡ������ƷID�б���ⲿ����
std::vector<unsigned int> Room::getLootFromEnemy(unsigned int enemyId) const {
	//��������������������������������������
	//TODO:���õ���or�������ݿ⺯�������ݵ���ID��ȡ������ƷID
	

	// �����ģ�⣬���ݵ���ID����һЩ����ĵ�����ƷID
	if (enemyId == 2001) { // �������ID 2001 ������ƷID 1001 �� 1002
		return { 1001, 1002 };
	}
	else if (enemyId == 2002) { // �������ID 2002 ������ƷID 1003
		return { 1003 };
	}
	// Ĭ��û�е���
	return {};   //��֮��󷵻ص�����Ʒ���б�
}

std::vector<unsigned int> Room::getLootFromNPC(unsigned int npcId) const {
	//����������������������������������������
	// //TODO:����NPC or NPC���ݿ⺯��������NPC ID��ȡ������ƷID
	// �����ģ�⣬����NPC ID����һЩ����ĵ�����ƷID
	if (npcId == 3001) { // ����NPC ID 3001 ������ƷID 1101
		return { 1101 };
	}
	else if (npcId == 3002) { // ����NPC ID 3002 ������ƷID 1102 �� 1103
		return { 1102, 1103 };
	}
	// Ĭ��û�е���
	return {}; //��֮��󷵻ص�����Ʒ���б�
}