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
    std::string des; //������Ϣ����
	std::map<DIRECTION, unsigned int> exits; //���ڣ�����->����ID
	std::vector<unsigned int> items; //��������ƷID
	std::vector<unsigned int> enemys; //�����ڵ���ID
	std::vector<unsigned int> npcs; //������NPC ID


public:

	Room();
	Room(const std::string& name, unsigned int id, RoomType type, const std::string& des);
	
	//������
	RoomType getType() const;
	const std::string& getDes() const;

	//�޸���
	void setType(RoomType type);
	void setDes(const std::string& des);

	//�������
	void addExit(DIRECTION dir, unsigned int roomId); //��ӳ���
	const std::map<DIRECTION, unsigned int>& getExits() const; //��ȡ���г���
	bool removeExit(DIRECTION); //�Ƴ�����
	bool hasExit(DIRECTION) const; //�Ƿ��и÷���ĳ���
	unsigned int getExit(DIRECTION) const; //��ȡ�÷���ķ���ID�����޸÷��򷵻�0

	//��Ʒ���
	void addItem(unsigned int itemId); //�����Ʒ
	bool removeItem(unsigned int itemId); //�Ƴ���Ʒ
	bool hasItem(unsigned int itemId) const; //�Ƿ��и���Ʒ
	const std::vector<unsigned int>& getItems() const; //��ȡ������������ƷID

	//�������
	void addEnemy(unsigned int enemyId); //��ӵ���
	bool removeEnemy(unsigned int enemyId); //�Ƴ�����
	bool hasEnemy(unsigned int enemyId) const; //�Ƿ��иõ���
	const std::vector<unsigned int>& getEnemys() const; //��ȡ���������е���ID

	//NPC���
	void addNPC(unsigned int npcId); //���NPC
	bool removeNPC(unsigned int npcId); //�Ƴ�NPC
	bool hasNPC(unsigned int npcId) const; //�Ƿ��и�NPC
	const std::vector<unsigned int>& getNPCs() const; //��ȡ����������NPC ID

	// JSON ��д��������Ϊ���󡣾��ǽ�ÿ��Room����ת��ΪJSON����
	void toJson(json& j) const override;  //
	void fromJson(const json& j) override;

	~Room() override;
};

