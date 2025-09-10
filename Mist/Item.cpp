#include "Item.h"

Item::Item(std::string name, std::string description, ItemEffect effect, int value)
    : name(name), description(description), effect(effect), value(value) {}