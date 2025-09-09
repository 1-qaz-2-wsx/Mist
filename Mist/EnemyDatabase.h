#pragma once

#include <string>
#include <unordered_map>
#include <memory> // For std::unique_ptr
#include "Enemy.h"

class EnemyDatabase {
public:
    bool load(const std::string& filename);

    // 获取敌人模板的常量指针
    const Enemy* getEnemyTemplate(unsigned int enemyId) const;

    /**
     * @brief 根据敌人ID创建一个新的敌人实例（副本）
     * @param enemyId 敌人的唯一ID
     * @return 一个指向新Enemy实例的unique_ptr，如果找不到则为空
     */
    std::unique_ptr<Enemy> createInstance(unsigned int enemyId) const;

private:
    std::unordered_map<unsigned int, Enemy> enemyTemplates_;
};