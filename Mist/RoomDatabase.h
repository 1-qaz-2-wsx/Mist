#pragma once
#include <memory>
#include <map>
#include <string>

class Room;

class RoomDatabase
{
private:
	std::map<unsigned int, std::shared_ptr<Room>> rooms; //房间ID->房间指针
	
public:
	RoomDatabase();
	RoomDatabase(const std::string& filename); //构造时可直接从文件加载


	//添加房间
	bool addRoom(const std::shared_ptr<Room>& room); //添加房间，若ID已存在返回false
	//移除房间
	bool removeRoom(unsigned int roomId); //移除房间，若ID不存在返回false

	//获取房间
	std::shared_ptr<Room> getRoom(unsigned int roomId) const; //获取房间，若ID不存在返回nullptr
	//检查房间是否存在
	bool hasRoom(unsigned int roomId) const; //检查房间是否存在
	//获取所有房间
	const std::map<unsigned int, std::shared_ptr<Room>>& getAllRooms() const; //获取所有房间
	
	// JSON 读写，就是将数据库储存的所有Room读写到文件，所以函数里会调用Room成员函数，以将每个Room读写到文件。
	bool loadFromFile(const std::string& filename); //从文件加载房间数据  填充rooms
	bool saveToFile(const std::string& filename) const; //保存房间数据到文件，json文件里以数组形式储存。


	//析构函数
	~RoomDatabase();



};

