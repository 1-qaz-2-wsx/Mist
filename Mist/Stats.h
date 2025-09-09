#pragma once

#include "json.hpp" // 包含json库

using json = nlohmann::json;

struct Stats {
    int maxHp = 0;
    int attack = 0;
    int defense = 0;

    // 为了方便，直接在这里实现 fromJson/toJson
    void fromJson(const json& j) {
        // 使用 .value(key, defaultValue) 是一个更安全的方法，
        // 如果json中缺少某个字段，它不会抛出异常，而是使用默认值。
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