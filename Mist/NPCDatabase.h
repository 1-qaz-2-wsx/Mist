#pragma once
#include <map>
#include <memory>
#include <string>
#include "NPC.h"

class NPCDatabase {
private:
    std::map<unsigned int, std::shared_ptr<NPC>> npcs;

public:
    bool addNPC(const std::shared_ptr<NPC>& npc);
    bool removeNPC(unsigned int npcId);
    std::shared_ptr<NPC> getNPC(unsigned int npcId) const;
    bool hasNPC(unsigned int npcId) const;

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
};
