#pragma once

#include "Room.h"
#include "Item.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <map>
#include <string> // ȷ�������� <string>

// ���������ȡ�� rooms.json �� map.json
inline std::vector<Room> createWorldRooms() {
    std::vector<Room> rooms;

    // --- ���� 1: ���������� ---
    Room room1;
    room1.id_ = 1;
    room1.name_ = "����������";
    // [��������] ʹ����򵥡��ȫ�ķ�ʽ�����ַ���
    std::string desc1 = "��վ��һ��ҡҡ��׹��������������ǽ���ϲ������ѷ졣";
    desc1 += "�ϱߺͶ��߸���һ��·��";
    room1.description = desc1;
    room1.x = 0;
    room1.y = 0;
    room1.exits = { {"south", 3}, {"east", 2} };

    auto key = std::make_unique<Item>();
    key->id_ = 3;
    key->name_ = "����Կ��";
    key->description = "һ�ѳ��ص���Կ�ף�������Ź��ϵķ��ġ�";
    room1.items.push_back(std::move(key));
    rooms.push_back(std::move(room1));


    // --- ���� 2: ������ɭ�� ---
    Room room2;
    room2.id_ = 2;
    room2.name_ = "������ɭ��";
    // [��������] ʹ����򵥡��ȫ�ķ�ʽ�����ַ���
    std::string desc2 = "�������һƬ������ɭ�֣��ߴ����ľ�ڱ������⡣";
    desc2 += "һֻ�粼������������ؿ����㡣�������������ϱ���һ��С����";
    room2.description = desc2;
    room2.x = 1;
    room2.y = 0;
    room2.exits = { {"south", 4}, {"west", 1} };

    auto goblin = std::make_unique<Enemy>();
    goblin->id_ = 101;
    goblin->name_ = "�粼��";
    goblin->baseStats = { 20, 5, 2 };
    goblin->currentHp = 20;
    goblin->lootTable = { 2 };
    room2.enemies.push_back(std::move(goblin));
    rooms.push_back(std::move(room2));


    // --- ���� 3: ����̦޺��ͥԺ ---
    Room room3;
    room3.id_ = 3;
    room3.name_ = "����̦޺��ͥԺ";
    // [��������] ʹ����򵥡��ȫ�ķ�ʽ�����ַ���
    std::string desc3 = "����һ����������ͥԺ��������������ʪ����̦޺��";
    desc3 += "����ͨ�������������ƺ���·��";
    room3.description = desc3;
    room3.x = 0;
    room3.y = 1;
    room3.exits = { {"north", 1}, {"east", 4} };

    auto potion = std::make_unique<Item>();
    potion->id_ = 2;
    potion->name_ = "����ҩˮ";
    potion->description = "һƿ��ɫ��Һ�壬�ƺ��������˿ڡ�";
    room3.items.push_back(std::move(potion));
    rooms.push_back(std::move(room3));


    // --- ���� 4: ��Ţ��С�� ---
    Room room4;
    room4.id_ = 4;
    room4.name_ = "��Ţ��С��";
    room4.description = "һ����Ţ��С������������졣������ɭ�֣�������ͥԺ��";
    room4.x = 1;
    room4.y = 1;
    room4.exits = { {"north", 2}, {"west", 3} };

    auto slime = std::make_unique<Enemy>();
    slime->id_ = 102;
    slime->name_ = "ʷ��ķ";
    slime->baseStats = { 15, 3, 5 };
    slime->currentHp = 15;
    room4.enemies.push_back(std::move(slime));
    rooms.push_back(std::move(room4));

    return rooms;
}