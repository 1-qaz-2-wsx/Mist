#include "Room.h"
#include "Item.h"
#include "Enemy.h"
#include "NPC.h"
#include <iostream>

Room::Room(std::string name, std::string description)
    : name(name), description(description), enemy(nullptr), npc(nullptr), item(nullptr) {}

// Room的析构函数，负责释放动态分配的内存
Room::~Room() {
    delete enemy;
    delete npc;
    delete item;
}

void Room::look() const {
    std::cout << description << "\n";

    if (enemy != nullptr) {
        std::cout << "这里有一个可怕的 " << enemy->name << "！\n";
    }
    if (npc != nullptr) {
        std::cout << "你看到 " << npc->name << " 站在这里。\n";
    }
    if (!items.empty()) {
        std::cout << "地上有以下物品:\n";
        for (const auto& item : items) {
            std::cout << "- " << item->name << "\n";
        }
    }

    std::cout << "你可以前往的方向: ";
    for (const auto& exit : exits) {
        std::cout << exit.first << " ";
    }
    std::cout << "\n";
}

void Room::takeItem(Item* item) {
    items.push_back(item);
}
std::string Room::getName() {
	return name;
}