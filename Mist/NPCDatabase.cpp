#include "NPCDatabase.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

bool NPCDatabase::addNPC(const std::shared_ptr<NPC>& npc) {
    if (npcs.find(npc->getId()) != npcs.end()) {
        return false; // ID �Ѵ���
    }
    npcs[npc->getId()] = npc;
    return true;
}

bool NPCDatabase::removeNPC(unsigned int npcId) {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) {
        return false; // ID ������
    }
    npcs.erase(it);
    return true;
}

std::shared_ptr<NPC> NPCDatabase::getNPC(unsigned int npcId) const {
    auto it = npcs.find(npcId);
    if (it == npcs.end()) {
        return nullptr;
    }
    return it->second;
}

bool NPCDatabase::hasNPC(unsigned int npcId) const {
    return npcs.find(npcId) != npcs.end();
}

bool NPCDatabase::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        return false; // �ļ���ʧ��
    }

    json j;
    inFile >> j;
    inFile.close();

    if (!j.is_array()) {
        return false; // ��ʽ����
    }

    npcs.clear();
    for (const auto& item : j) {
        auto npc = std::make_shared<NPC>();
        npc->fromJson(item);
        npcs[npc->getId()] = npc;
    }
    return true;
}

bool NPCDatabase::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        return false; // �ļ���ʧ��
    }

    json j = json::array();
    for (const auto& [id, npc] : npcs) {
        json npcJson;
        npc->toJson(npcJson);
        j.push_back(npcJson);
    }

    outFile << j.dump(4); // ��ʽ�����
    outFile.close();
    return true;
}
