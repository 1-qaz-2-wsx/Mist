#pragma once
#include <map>
#include <utility> // std::pair

using Coord = std::pair<int, int>; //坐标

class RoomDatabase;

class Map//只负责：坐标 → 房间ID 的映射、当前位置、移动规则。
{
private:
	unsigned int currentRoomId_; //当前房间ID
	RoomDatabase& db_;                       // 非拥有引用
	Coord player_{0,0}; //玩家坐标

	std::map<Coord, unsigned int> positionToRoomId; //坐标到房间ID的映射

public:
	Map(RoomDatabase& db);  //依赖注入RoomDatabase
	~Map();

	bool load(const std::string& filename); //从文件加载地图
	bool movePlayer(int x, int y);      // 移动玩家，返回是否成功
	unsigned int getCurrentRoomId() const;

	// 查询某坐标的房间ID，不存在时返回 0
	unsigned int roomAt(int x, int y) const;

	void printMap() const; //打印地图

};

