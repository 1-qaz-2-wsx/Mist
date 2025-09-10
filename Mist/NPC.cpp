#include "NPC.h"
#include <iostream>

NPC::NPC(std::string name, std::string dialogue, Item item)
    : name(name), dialogue(dialogue), itemToGive(item), hasGivenItem(false) {}

void NPC::talk() {
    std::cout << name << " 说: \"" << dialogue << "\"" << std::endl;
}

Item NPC::giveItem() {
    hasGivenItem = true;
    return itemToGive;
}