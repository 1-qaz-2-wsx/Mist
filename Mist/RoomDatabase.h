#pragma once
#include <memory>
#include <map>
#include <string>

class Room;

class RoomDatabase
{
private:
	std::map<unsigned int, std::shared_ptr<Room>> rooms; //����ID->����ָ��
	
public:
	RoomDatabase();
	RoomDatabase(const std::string& filename); //����ʱ��ֱ�Ӵ��ļ�����


	//��ӷ���
	bool addRoom(const std::shared_ptr<Room>& room); //��ӷ��䣬��ID�Ѵ��ڷ���false
	//�Ƴ�����
	bool removeRoom(unsigned int roomId); //�Ƴ����䣬��ID�����ڷ���false

	//��ȡ����
	std::shared_ptr<Room> getRoom(unsigned int roomId) const; //��ȡ���䣬��ID�����ڷ���nullptr
	//��鷿���Ƿ����
	bool hasRoom(unsigned int roomId) const; //��鷿���Ƿ����
	//��ȡ���з���
	const std::map<unsigned int, std::shared_ptr<Room>>& getAllRooms() const; //��ȡ���з���
	
	// JSON ��д�����ǽ����ݿⴢ�������Room��д���ļ������Ժ���������Room��Ա�������Խ�ÿ��Room��д���ļ���
	bool loadFromFile(const std::string& filename); //���ļ����ط�������  ���rooms
	bool saveToFile(const std::string& filename) const; //���淿�����ݵ��ļ���json�ļ�����������ʽ���档


	//��������
	~RoomDatabase();



};

