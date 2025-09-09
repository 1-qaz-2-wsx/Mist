#pragma once

#include <string>
#include <unordered_map>
#include <memory> // For std::unique_ptr
#include "Enemy.h"

class EnemyDatabase {
public:
    bool load(const std::string& filename);

    // ��ȡ����ģ��ĳ���ָ��
    const Enemy* getEnemyTemplate(unsigned int enemyId) const;

    /**
     * @brief ���ݵ���ID����һ���µĵ���ʵ����������
     * @param enemyId ���˵�ΨһID
     * @return һ��ָ����Enemyʵ����unique_ptr������Ҳ�����Ϊ��
     */
    std::unique_ptr<Enemy> createInstance(unsigned int enemyId) const;

private:
    std::unordered_map<unsigned int, Enemy> enemyTemplates_;
};