#pragma once

#include "Room.h"
#include "Item.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <map>
#include <string> // 确保包含了 <string>

// 这个函数将取代 rooms.json 和 map.json
inline std::vector<Room> createWorldRooms() {
    std::vector<Room> rooms;

    // --- 房间 1: 废弃的哨塔 ---
    Room room1;
    room1.id_ = 1;
    room1.name_ = "废弃的哨塔";
    // [最终修正] 使用最简单、最安全的方式定义字符串
    std::string desc1 = "你站在一座摇摇欲坠的哨塔里。尘土飞扬，墙壁上布满了裂缝。";
    desc1 += "南边和东边各有一条路。";
    room1.description = desc1;
    room1.x = 0;
    room1.y = 0;
    room1.exits = { {"south", 3}, {"east", 2} };

    auto key = std::make_unique<Item>();
    key->id_ = 3;
    key->name_ = "地牢钥匙";
    key->description = "一把沉重的铁钥匙，上面刻着古老的符文。";
    room1.items.push_back(std::move(key));
    rooms.push_back(std::move(room1));


    // --- 房间 2: 阴暗的森林 ---
    Room room2;
    room2.id_ = 2;
    room2.name_ = "阴暗的森林";
    // [最终修正] 使用最简单、最安全的方式定义字符串
    std::string desc2 = "你进入了一片阴暗的森林，高大的树木遮蔽了阳光。";
    desc2 += "一只哥布林正虎视眈眈地看着你。西边是哨塔，南边是一条小径。";
    room2.description = desc2;
    room2.x = 1;
    room2.y = 0;
    room2.exits = { {"south", 4}, {"west", 1} };

    auto goblin = std::make_unique<Enemy>();
    goblin->id_ = 101;
    goblin->name_ = "哥布林";
    goblin->baseStats = { 20, 5, 2 };
    goblin->currentHp = 20;
    goblin->lootTable = { 2 };
    room2.enemies.push_back(std::move(goblin));
    rooms.push_back(std::move(room2));


    // --- 房间 3: 长满苔藓的庭院 ---
    Room room3;
    room3.id_ = 3;
    room3.name_ = "长满苔藓的庭院";
    // [最终修正] 使用最简单、最安全的方式定义字符串
    std::string desc3 = "这是一个被遗忘的庭院，地面上铺满了湿滑的苔藓。";
    desc3 += "北边通往哨塔，东边似乎有路。";
    room3.description = desc3;
    room3.x = 0;
    room3.y = 1;
    room3.exits = { {"north", 1}, {"east", 4} };

    auto potion = std::make_unique<Item>();
    potion->id_ = 2;
    potion->name_ = "治疗药水";
    potion->description = "一瓶红色的液体，似乎能治愈伤口。";
    room3.items.push_back(std::move(potion));
    rooms.push_back(std::move(room3));


    // --- 房间 4: 泥泞的小径 ---
    Room room4;
    room4.id_ = 4;
    room4.name_ = "泥泞的小径";
    room4.description = "一条泥泞的小径在你脚下延伸。北边是森林，西边是庭院。";
    room4.x = 1;
    room4.y = 1;
    room4.exits = { {"north", 2}, {"west", 3} };

    auto slime = std::make_unique<Enemy>();
    slime->id_ = 102;
    slime->name_ = "史莱姆";
    slime->baseStats = { 15, 3, 5 };
    slime->currentHp = 15;
    room4.enemies.push_back(std::move(slime));
    rooms.push_back(std::move(room4));

    return rooms;
}