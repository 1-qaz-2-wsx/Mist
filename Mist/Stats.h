#pragma once

#include "json.hpp" // ����json��

using json = nlohmann::json;

struct Stats {
    int maxHp = 0;
    int attack = 0;
    int defense = 0;

    // Ϊ�˷��㣬ֱ��������ʵ�� fromJson/toJson
    void fromJson(const json& j) {
        // ʹ�� .value(key, defaultValue) ��һ������ȫ�ķ�����
        // ���json��ȱ��ĳ���ֶΣ��������׳��쳣������ʹ��Ĭ��ֵ��
        maxHp = j.value("maxHp", 0);
        attack = j.value("attack", 0);
        defense = j.value("defense", 0);
    }

    void toJson(json& j) const {
        j = {
            {"maxHp", maxHp},
            {"attack", attack},
            {"defense", defense}
        };
    }
};