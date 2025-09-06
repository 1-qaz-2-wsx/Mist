#pragma once
#include "Entity.h"
#include <map>
#include <vector>
enum RoomType {
	DEFAULT,
    FIGHT,
    STORE,
    ENCOUNTER,
    NUMROOM
};

enum DIRECTION {
	NORTH,
	SOUTH,
	EAST,
	WEST,

	UP,
	DOWN,
	IN,
	OUT,
	NUMDIR
};



class Room :
    public Entity
{
private:
    RoomType type;
    std::string des; //房间信息描述
	std::map<DIRECTION, unsigned int> exits; //出口，方向->房间ID
	std::vector<unsigned int> items; //房间内物品ID
	std::vector<unsigned int> enemys; //房间内敌人ID
	std::vector<unsigned int> npcs; //房间内NPC ID


public:

	Room();
	Room(const std::string& name, unsigned int id, RoomType type, const std::string& des);
	
	//访问器
	RoomType getType() const;
	const std::string& getDes() const;

	//修改器
	void setType(RoomType type);
	void setDes(const std::string& des);

	//出口相关
	void addExit(DIRECTION dir, unsigned int roomId); //添加出口
	const std::map<DIRECTION, unsigned int>& getExits() const; //获取所有出口
	bool removeExit(DIRECTION); //移除出口
	bool hasExit(DIRECTION) const; //是否有该方向的出口
	unsigned int getExit(DIRECTION) const; //获取该方向的房间ID，若无该方向返回0

	//物品相关
	void addItem(unsigned int itemId); //添加物品
	bool removeItem(unsigned int itemId); //移除物品
	bool hasItem(unsigned int itemId) const; //是否有该物品
	const std::vector<unsigned int>& getItems() const; //获取房间内所有物品ID

	//敌人相关
	void addEnemy(unsigned int enemyId); //添加敌人
	bool removeEnemy(unsigned int enemyId); //移除敌人
	bool hasEnemy(unsigned int enemyId) const; //是否有该敌人
	const std::vector<unsigned int>& getEnemys() const; //获取房间内所有敌人ID

	//NPC相关
	void addNPC(unsigned int npcId); //添加NPC
	bool removeNPC(unsigned int npcId); //移除NPC
	bool hasNPC(unsigned int npcId) const; //是否有该NPC
	const std::vector<unsigned int>& getNPCs() const; //获取房间内所有NPC ID

	// JSON 读写，调用者为对象。就是将每个Room对象转换为JSON对象
	void toJson(json& j) const override;  //
	void fromJson(const json& j) override;

	~Room() override;
};

