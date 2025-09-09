#include "EnemyDatabase.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool EnemyDatabase::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open enemy database file: " << filename << std::endl;
        return false;
    }

    json jsonData;
    try {
        file >> jsonData;
    }
    catch (const json::parse_error& e) {
        std::cerr << "Error: Failed to parse JSON file: " << filename << "\n"
            << "Parse error: " << e.what() << std::endl;
        return false;
    }

    enemyTemplates_.clear();

    for (const auto& enemyObject : jsonData) {
        Enemy newEnemy;
        newEnemy.fromJson(enemyObject);
        enemyTemplates_[newEnemy.getId()] = newEnemy;
    }

    std::cout << "Loaded " << enemyTemplates_.size() << " enemies from " << filename << std::endl;
    return true;
}

const Enemy* EnemyDatabase::getEnemyTemplate(unsigned int enemyId) const {
    auto it = enemyTemplates_.find(enemyId);
    if (it != enemyTemplates_.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::unique_ptr<Enemy> EnemyDatabase::createInstance(unsigned int enemyId) const {
    const Enemy* enemyTemplate = getEnemyTemplate(enemyId);
    if (enemyTemplate) {
        // ����һ��ģ��Ŀ����������Ҫ����Ϊÿ����������Ϸ��ĵ���
        // ����Ҫ���Լ������� currentHp ��״̬��
        return std::make_unique<Enemy>(*enemyTemplate);
    }
    return nullptr;
}