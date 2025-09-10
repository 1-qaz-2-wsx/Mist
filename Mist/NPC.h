#pragma once
#include <string>
#include "Item.h"

class NPC {
public:
    std::string name;
    std::string dialogue;
    Item itemToGive;
    bool hasGivenItem;

    NPC(std::string name, std::string dialogue, Item item);

    void talk();
    Item giveItem();
};