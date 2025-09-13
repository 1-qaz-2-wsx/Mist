#include "Map.h"
#include "Enemy.h"
#include "NPC.h"
#include "SlowPrint.h"
#include <iostream>
#include <random> // 用于随机数
#include <fstream>      // *** 新增：用于文件输入流 ***
#include <sstream>      // *** 新增：用于字符串流 ***

Map::Map() : startRoom(nullptr), hotelRoom(nullptr) {
    buildItemDatabase(); // 初始化时就构建好物品数据库
    loadMapFromFile("map.txt"); // 在构造时自动加载地图文件
}

void Map::buildItemDatabase() {

    // 消耗品类物品（含各类buff、恢复类）
    itemDatabase["迷雾核心"] = Item("迷雾核心", "一个强大的能量源", ItemType::CONSUMABLE, ItemEffect::ATTACK_BUFF, 20);
    itemDatabase["生命药水"] = Item("生命药水", "恢复70点生命值", ItemType::CONSUMABLE, ItemEffect::HEAL, 70); // 统一为70点恢复（覆盖原50点的重复定义）
    itemDatabase["哥布林的牙齿"] = Item("哥布林的牙齿", "坚硬的牙齿，使用后永久提升5点防御力", ItemType::CONSUMABLE, ItemEffect::DEFENSE_BUFF, 5); // 统一为防御buff（覆盖原“战利品/5点恢复”的重复定义）
    itemDatabase["食尸鬼的利爪"] = Item("食尸鬼的利爪", "锋利的爪子，使用后永久提升7点攻击力。", ItemType::CONSUMABLE, ItemEffect::ATTACK_BUFF, 10);
    itemDatabase["迅捷符文"] = Item("迅捷符文", "古老的符文，使用后永久提升5点敏捷。", ItemType::CONSUMABLE, ItemEffect::AGILITY_BUFF, 5);
    itemDatabase["智慧卷轴"] = Item("智慧卷轴", "蕴含古老知识的卷轴，使用后永久提升5点智力。", ItemType::CONSUMABLE, ItemEffect::INTELLIGENCE_BUFF, 5);
    itemDatabase["月光珍珠"] = Item("月光珍珠", "闪耀着月色的珍珠，使用后永久提升10点智力。", ItemType::CONSUMABLE, ItemEffect::INTELLIGENCE_BUFF, 10);
    itemDatabase["沼泽毒牙"] = Item("沼泽毒牙", "带有剧毒的獠牙，使用后永久提升10点攻击力。", ItemType::CONSUMABLE, ItemEffect::ATTACK_BUFF, 10);
    itemDatabase["使用手册"] = Item("使用手册", "阅读后提升武器熟练度", ItemType::CONSUMABLE, ItemEffect::WEAPON_PROFICIENCY_BUFF, 5);

    // 武器类物品（需传入武器伤害参数，对应 Item 构造的第6个参数）
    itemDatabase["生锈的剑"] = Item("生锈的剑", "增加10点攻击力", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 10);
    itemDatabase["猎枪"] = Item("猎枪", "一把老旧的猎枪", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 15);
    itemDatabase["法杖"] = Item("法杖", "一根普通的木制法杖", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 8);
    itemDatabase["重剑"] = Item("重剑", "一把沉重的双手大剑", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 12);
    itemDatabase["龙骨大剑"] = Item("龙骨大剑", "由幼龙骨制成的巨剑", ItemType::WEAPON, ItemEffect::ATTACK_BUFF, 0, 25);
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

// *** 新增：loadMapFromFile 函数的实现 ***
void Map::loadMapFromFile(const std::string& filePath) {
    std::ifstream mapFile(filePath);
    if (mapFile.is_open()) {
        std::stringstream buffer;
        buffer << mapFile.rdbuf();
        mapArt = buffer.str();
        mapFile.close();
    }
    else {
        // 如果文件加载失败，提供一个默认的硬编码地图作为备用
        mapArt = "[地图文件 map.txt 加载失败]\n";
        std::cout << "        +------------------+\n";
        std::cout << "        |    宁静空地      |\n";
        std::cout << "        |                  |\n";
        std::cout << "        +------------------+\n";
        std::cout << "                | \n";
        std::cout << "                |\n";
        std::cout << "+------------------+         +------------------+         +------------------+ \n";
        std::cout << "|    低语森林      |-------->|    神秘洞穴      |-------->|    黑暗沼泽      |\n";
        std::cout << "|                  |         |                  |         |                  |\n";
        std::cout << "+------------------+         +------------------+         +------------------+\n";
        std::cout << "        |                            | \n";
        std::cout << "        | 北                         |\n";
        std::cout << "+------------------+         +------------------+         +------------------+\n";
        std::cout << "|  初始之地        |-------->|    废弃矿洞      |-------->|  月光湖畔        |\n";
        std::cout << "|                  |   东    |                  |         |                  |\n";
        std::cout << "+------------------+         +------------------+         +------------------+ \n";
        std::cout << "        | 南                             |   \n";
        std::cout << "        |                                |\n";
        std::cout << "+----------------------+           +-------------------+\n";
        std::cout << "| 金碧辉煌的饭店(hotel)|---------->|    荒芜高地       |\n";
        std::cout << "|                      |           |                   |\n";
        std::cout << "+----------------------+           +-------------------+\n";
            
    }
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
    Item* dragonBone = new Item(getItemFromDatabase("龙骨大剑"));
    Item* moonPearl = new Item(getItemFromDatabase("月光珍珠"));
    Item* swampFang = new Item(getItemFromDatabase("沼泽毒牙"));
    Item* wisdomScroll = new Item(getItemFromDatabase("沼泽毒牙"));
    Item* swiftRune = new Item(getItemFromDatabase("迅捷符文"));
    Item* ghoulClaw = new Item(getItemFromDatabase("食尸鬼的利爪"));


    // 2. 创建房间
    Room* r1 = new Room("初始之地", "你身处一个昏暗的【初始之地】，四周弥漫着薄雾。");
    Room* r2 = new Room("低语森林", "这里是【低语森林】，偶尔能听到怪物的嘶吼。");
    Room* r3 = new Room("宁静空地", "你来到一片【宁静空地】，中央有一个和蔼的【老人】。");
    Room* hotel = new Room("金碧辉煌的饭店", "你走进一家【金碧辉煌的饭店】，里面的食物香气扑鼻。");
    this->hotelRoom = hotel; // 将创建的饭店赋值给成员指针
    Room* r4 = new Room("神秘洞穴", "你进入了一个阴暗的【神秘洞穴】，四周回荡着水滴声。");
    Room* r5 = new Room("废弃矿洞", "你来到一个【废弃矿洞】，空气中弥漫着潮湿和铁锈的味道，脚下是摇摇欲坠的木板。");
    Room* r6 = new Room("月光湖畔", "银色月光洒在【月光湖畔】，湖水波光粼粼。");
    Room* r7 = new Room("黑暗沼泽", "【黑暗沼泽】散发着腐败的气息，空气中带着剧毒。");
    Room* r8 = new Room("荒芜高地", "你登上【荒芜高地】，寒风呼啸，视野极为开阔。");

    // 3. 创建生物 (Loot现在直接从数据库获取)
    Enemy* goblin = new Enemy("哥布林", 40, 10, 2, *goblinLoot);
    Enemy* ghoul = new Enemy("矿洞食尸鬼", 60, 13, 5, *ghoulClaw);
    Enemy* swampBeast = new Enemy("沼泽巨兽", 70, 15, 3, *swampFang);
    Enemy* highlandDrake = new Enemy("高地幼龙", 80, 15, 8, *dragonBone);
    NPC* oldMan = new NPC("老人", "年轻人，这瓶药水你拿去吧，路上要小心。", *healthPotion);
    NPC* scholar = new NPC("神秘学者", "……原来有人能来到这里。这本手册交给你，好好使用吧。", *manual);
    NPC* moonMaiden = new NPC("月光少女", "月色可以洗炼心灵，这颗珍珠赠与你。", *moonPearl);

    // 4. 将生物和物品放入房间
    r1->takeItem(huntingGun);
    r1->takeItem(staff);
    r1->takeItem(greatsword);
    //r1->takeItem(manual);
    r2->takeItem(swiftRune);
    r4->takeItem(wisdomScroll);
    r2->enemy = goblin;
    r3->npc = oldMan;
    r4->npc = scholar;
    r5->enemy = ghoul;
    r6->npc = moonMaiden;
    r7->enemy = swampBeast;
    r8->enemy = highlandDrake;


    // 5. 连接房间
    r1->exits["north"] = r2;
    r1->exits["south"] = hotel;
    r1->exits["east"] = r5;
    r2->exits["south"] = r1;
    r2->exits["north"] = r3;
    r2->exits["east"] = r4;
    r3->exits["south"] = r2;
    hotel->exits["north"] = r1;
    r4->exits["west"] = r2;
    r4->exits["south"] = r5;
    r4->exits["east"] = r7;
    r5->exits["north"] = r4;
    r5->exits["west"] = r1;
    r5->exits["east"] = r6;
    r5->exits["south"] = r8;
    r6->exits["west"] = r5;
    r7->exits["west"] = r4;
    r8->exits["west"] = hotel;
    r8->exits["north"] = r5;

    // 6. 设置起点
    startRoom = r1;

    // 7. 将所有房间加入管理器以便释放内存
    allRooms.push_back(r1);
    allRooms.push_back(r2);
    allRooms.push_back(r3);
    allRooms.push_back(hotel);
    allRooms.push_back(r4);
    allRooms.push_back(r5);
    allRooms.push_back(r6);
    allRooms.push_back(r7);
    allRooms.push_back(r8);

}

void Map::printMap() const { 
    slowPrint("当前地图:\n", 5);

    std::cout << mapArt << std::endl; // 直接打印存储的地图内容
}