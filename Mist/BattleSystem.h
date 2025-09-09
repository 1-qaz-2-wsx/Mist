#pragma once
#include <vector>
class Player;
class Room;

class BattleSystem
{
public:
	std::vector<unsigned> fight(Player& player, Room& room); // ·µ»ØµôÂäÎïÆ·
};

