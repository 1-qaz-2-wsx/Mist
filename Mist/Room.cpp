#include "Room.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
//构造函数
Room::Room() : Entity(), type(DEFAULT), des("A nondescript room.") {}
Room::Room(const std::string& name, unsigned int id, RoomType type, const std::string& des)
	: Entity(name, id), type(type), des(des) {}

//访问器
RoomType Room::getType() const {
	return type;
}

const std::string& Room::getDes() const {
	return des;
}

//修改器
void Room::setType(RoomType type) {
	this->type = type;
}

void Room::setDes(const std::string& des) {
	this->des = des;
}

//出口相关
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
	return 0; //若无该方向返回0
}

//物品相关
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

//敌人相关
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

//NPC相关
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


// JSON 读写
void Room::toJson(json& j) const {
	Entity::toJson(j); //调用基类的toJson，先写name 和id
	// 房间特有属性
	j["type"] = static_cast<int>(type);
	j["description"] = des;

	// 出口
	j["exits"] = json::object();
	for (const auto& [dir, roomId] : exits) {
		j["exits"][std::to_string(static_cast<int>(dir))] = roomId;
	}

	// 物品
	j["items"] = items;

	// 敌人
	j["enemys"] = enemys;

	// NPC
	j["npcs"] = npcs;
}

void Room::fromJson(const json& j) {
	Entity::fromJson(j); //调用基类的fromJson，先读name 和id

	// 房间特有属性
	if (j.contains("type") && j["type"].is_number_integer()) {
		type = static_cast<RoomType>(j["type"].get<int>());
	}
	if (j.contains("description") && j["description"].is_string()) {
		des = j["description"].get<std::string>();
	}

	// 出口
	exits.clear(); //先清空现有出口，避免数据残留
	if (j.contains("exits") && j["exits"].is_object()) {
		for (auto& [key, value] : j["exits"].items()) {
			if (value.is_number_unsigned()) {
				DIRECTION dir = static_cast<DIRECTION>(std::stoi(key));
				exits[dir] = value.get<unsigned int>();
			}
		}
	}

	// 物品
	items.clear();
	if (j.contains("items") && j["items"].is_array()) {
		for (const auto& item : j["items"]) {
			if (item.is_number_unsigned()) {
				items.push_back(item.get<unsigned int>());
			}
		}
	}

	// 敌人
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
	// 外部函数根据敌人ID获取掉落物品ID列表
	std::vector<unsigned int> loot;

	// 根据房间类型判断是敌人掉落、NPC掉落，还是商店物品
	if (type == RoomType::FIGHT) {
		// 如果房间类型是战斗（FIGHT），敌人掉落物品
		loot = dropLootFromEnemies();
	}
	else if (type == RoomType::ENCOUNTER) {
		// 如果房间类型是偶遇（ENCOUNTER），NPC掉落物品
		loot = dropLootFromNPCs();
	}
	else if (type == RoomType::STORE) {
		// 商店类型的房间通常不掉落物品，而是玩家可以购买
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


//。。。。。。。。。。。。。。。。。
std::vector<unsigned int> Room::dropLootFromStore() const {
	std::vector<unsigned int> loot;
	// 商店的物品可以是预设好的库存，假设是库存物品ID
	loot.push_back(4001); // 假设商店有ID为4001的物品出售   //这里可以根据实际需求调整
	return loot;
}
//。。。。。。。。。。。。。。。。。



// 模拟根据敌人ID获取掉落物品ID列表的外部函数
std::vector<unsigned int> Room::getLootFromEnemy(unsigned int enemyId) const {
	//。。。。。。。。。。。。。。。。。。。
	//TODO:调用敌人or敌人数据库函数，根据敌人ID获取掉落物品ID
	

	// 这里简单模拟，根据敌人ID返回一些假设的掉落物品ID
	if (enemyId == 2001) { // 假设敌人ID 2001 掉落物品ID 1001 和 1002
		return { 1001, 1002 };
	}
	else if (enemyId == 2002) { // 假设敌人ID 2002 掉落物品ID 1003
		return { 1003 };
	}
	// 默认没有掉落
	return {};   //总之最后返回掉落物品的列表
}

std::vector<unsigned int> Room::getLootFromNPC(unsigned int npcId) const {
	//。。。。。。。。。。。。。。。。。。。。
	// //TODO:调用NPC or NPC数据库函数，根据NPC ID获取掉落物品ID
	// 这里简单模拟，根据NPC ID返回一些假设的掉落物品ID
	if (npcId == 3001) { // 假设NPC ID 3001 掉落物品ID 1101
		return { 1101 };
	}
	else if (npcId == 3002) { // 假设NPC ID 3002 掉落物品ID 1102 和 1103
		return { 1102, 1103 };
	}
	// 默认没有掉落
	return {}; //总之最后返回掉落物品的列表
}