#include "Map.h"
#include "Enemy.h"
#include "NPC.h"
#include "SlowPrint.h"
#include <iostream>

Map::Map() : startRoom(nullptr) {}

Map::~Map() {
    for (Room* room : allRooms) {
        delete room;
    }
    allRooms.clear();
}


void Map::build() {
    // 1. 创建物品
    Item* healthPotion = new Item("生命药水", "恢复50点生命值", ItemEffect::HEAL, 50);
    Item* rustySword = new Item("生锈的剑", "增加10点攻击力", ItemEffect::ATTACK_BUFF, 10);
    Item* goblinLoot = new Item("哥布林的牙齿", "一个战利品", ItemEffect::HEAL, 5); // 哥布林掉落物

    // 2. 创建房间
    Room* r1 = new Room("你身处一个昏暗的【初始之地】，四周弥漫着薄雾。");
    Room* r2 = new Room("这里是【低语森林】，偶尔能听到怪物的嘶吼。北边有一片空地。");
    Room* r3 = new Room("你来到一片【宁静空地】，中央有一个和蔼的【老人】。西边是森林。");

    // 3. 创建生物
    Enemy* goblin = new Enemy("哥布林", 40, 10, 2, *goblinLoot);
    NPC* oldMan = new NPC("老人", "年轻人，这瓶药水你拿去吧，路上要小心。", *healthPotion);

    // 4. 将生物和物品放入房间
    r2->enemy = goblin;
    r3->npc = oldMan;
    // r1->item = rustySword; // 比如在初始地放一把剑

    // 5. 连接房间
    r1->exits["north"] = r2;
    r2->exits["south"] = r1;
    r2->exits["north"] = r3;
    r3->exits["south"] = r2;

    // 6. 设置起点
    startRoom = r1;

    // 7. 将所有房间加入管理器以便释放内存
    allRooms.push_back(r1);
    allRooms.push_back(r2);
    allRooms.push_back(r3);

    // 注意：这里动态分配的Item, Enemy, NPC需要在Room的析构函数中被delete
}

void Map::printMap() {
	slowPrint("地图构建完成！你现在可以开始你的冒险了。\n",50);
	slowPrint("当前地图:\n", 50);
	std::cout << "[初始之地] -- 北 --> [低语森林] -- 北 --> [宁静空地]\n";
	std::cout << "                     |                      \n";
	std::cout << "                     南                     \n";
    std::cout << "                     |                      \n";
    std::cout << "                  [初始之地]                \n";
}

