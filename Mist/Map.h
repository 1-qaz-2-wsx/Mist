#pragma once
#include <map>
#include <utility> // std::pair

using Coord = std::pair<int, int>; //����

class RoomDatabase;

class Map//ֻ�������� �� ����ID ��ӳ�䡢��ǰλ�á��ƶ�����
{
private:
	unsigned int currentRoomId_; //��ǰ����ID
	RoomDatabase& db_;                       // ��ӵ������
	Coord player_{0,0}; //�������

	std::map<Coord, unsigned int> positionToRoomId; //���굽����ID��ӳ��

public:
	Map(RoomDatabase& db);  //����ע��RoomDatabase
	~Map();

	bool load(const std::string& filename); //���ļ����ص�ͼ
	bool movePlayer(int x, int y);      // �ƶ���ң������Ƿ�ɹ�
	unsigned int getCurrentRoomId() const;

	// ��ѯĳ����ķ���ID��������ʱ���� 0
	unsigned int roomAt(int x, int y) const;

	void printMap() const; //��ӡ��ͼ

};

