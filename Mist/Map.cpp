#include "Map.h"
#include "Enemy.h"
#include "NPC.h"
#include "SlowPrint.h"
#include <iostream>
#include <random> // 用于随机数

Map::Map() : startRoom(nullptr), hotelRoom(nullptr) {
    buildItemDatabase(); // 初始化时就构建好物品数据库
}

void Map::buildItemDatabase() {
    // 这里我们定义游戏中所有可能出现的物品
    itemDatabase["生命药水"] = Item("生命药水", "恢复50点生命值", ItemType::CONSUMABLE, ItemEffect::HEAL, 50);
    itemDatabase["生锈的剑"] = Item("生锈的剑", "增加10点攻击力", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 10);
    itemDatabase["哥布林的牙齿"] = Item("哥布林的牙齿", "一个战利品", ItemType::CONSUMABLE, ItemEffect::HEAL, 5);
    itemDatabase["猎枪"] = Item("猎枪", "一把老旧的猎枪", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 15);
    itemDatabase["法杖"] = Item("法杖", "一根普通的木制法杖", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 8);
    itemDatabase["重剑"] = Item("重剑", "一把沉重的双手大剑", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 12);
    itemDatabase["使用手册"] = Item("使用手册", "阅读后提升武器熟练度", ItemType::CONSUMABLE, ItemEffect::WEAPON_PROFICIENCY_BUFF, 5);
    itemDatabase["迷雾核心"] = Item("迷雾核心", "一个强大的能量源", ItemType::CONSUMABLE, ItemEffect::ATTACK_BUFF, 20);
}

Map::~Map() {
    for (Room* room : allRooms) {
        delete room;
    }
    allRooms.clear();
}

Item Map::getItemFromDatabase(const std::string& itemName) const {
    auto it = itemDatabase.find(itemName);
    if (it != itemDatabase.end()) {
        return it->second; // 返回找到的物品的拷贝
    }
    return Item(); // 如果找不到，返回一个空的默认物品
}

void Map::build() {

    // 清理旧的世界数据，以便重新加载
    for (Room* room : allRooms) {
        delete room;
    }
    allRooms.clear();

    // 1. 从数据库获取物品原型来创建实例
    // 注意：因为物品是放在房间里的，需要动态分配内存
    Item* healthPotion = new Item(getItemFromDatabase("生命药水"));
    Item* rustySword = new Item(getItemFromDatabase("生锈的剑"));
    Item* goblinLoot = new Item(getItemFromDatabase("哥布林的牙齿"));
    Item* huntingGun = new Item(getItemFromDatabase("猎枪"));
    Item* staff = new Item(getItemFromDatabase("法杖"));
    Item* greatsword = new Item(getItemFromDatabase("重剑"));
    Item* manual = new Item(getItemFromDatabase("使用手册"));

    // 2. 创建房间
    Room* r1 = new Room("初始之地", "你身处一个昏暗的【初始之地】，四周弥漫着薄雾。");
    Room* r2 = new Room("低语森林", "这里是【低语森林】，偶尔能听到怪物的嘶吼。");
    Room* r3 = new Room("宁静空地", "你来到一片【宁静空地】，中央有一个和蔼的【老人】。");
    Room* hotel = new Room("金碧辉煌的饭店", "你走进一家【金碧辉煌的饭店】，里面的食物香气扑鼻。");
    this->hotelRoom = hotel;

    // 3. 创建生物 (Loot现在直接从数据库获取)
    Enemy* goblin = new Enemy("哥布林", 40, 10, 2, getItemFromDatabase("哥布林的牙齿"));
    NPC* oldMan = new NPC("老人", "年轻人，这瓶药水你拿去吧，路上要小心。", getItemFromDatabase("生命药水"));

    // 4. 将生物和物品放入房间
    r1->takeItem(huntingGun);
    r1->takeItem(staff);
    r1->takeItem(greatsword);
    r1->takeItem(manual);
    // r1->item = rustySword; // 这行是旧代码，可以删掉

    r2->enemy = goblin;
    r3->npc = oldMan;



    // 5. 连接房间
    r1->exits["north"] = r2;
    r1->exits["east"] = hotel;
    r2->exits["south"] = r1;
    r2->exits["north"] = r3;
    r3->exits["south"] = r2;
    hotel->exits["west"] = r1;

    // 6. 设置起点
    startRoom = r1;

    // 7. 将所有房间加入管理器以便释放内存
    allRooms.push_back(r1);
    allRooms.push_back(r2);
    allRooms.push_back(r3);
    allRooms.push_back(hotel);
}

void Map::printMap() {
	//slowPrint("地图构建完成！你现在可以开始你的冒险了。\n",50);
	slowPrint("当前地图:\n", 10);
	std::cout << "[初始之地] -- 北 --> [低语森林] -- 北 --> [宁静空地]\n";
	std::cout << "                     |                      \n";
	std::cout << "                     南                     \n";
    std::cout << "                     |                      \n";
    std::cout << "                  [初始之地]                \n";
}

