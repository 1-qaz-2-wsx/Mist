// Exit.h
#pragma once
#include "json.hpp"

using json = nlohmann::json;

struct Exit {
    unsigned int destinationRoomId = 0;
    bool isLocked = false;
    unsigned int keyId = 0; // 0 代表不需要钥匙

    // 让 Exit 自己从 JSON 对象中加载数据
    void fromJson(const json& j) {
        destinationRoomId = j.at("destination").get<unsigned int>();
        isLocked = j.value("locked", false); // .value() 更安全，若字段不存在则使用默认值
        keyId = j.value("keyId", 0);
    }
};