// Exit.h
#pragma once
#include "json.hpp"

using json = nlohmann::json;

struct Exit {
    unsigned int destinationRoomId = 0;
    bool isLocked = false;
    unsigned int keyId = 0; // 0 ������ҪԿ��

    // �� Exit �Լ��� JSON �����м�������
    void fromJson(const json& j) {
        destinationRoomId = j.at("destination").get<unsigned int>();
        isLocked = j.value("locked", false); // .value() ����ȫ�����ֶβ�������ʹ��Ĭ��ֵ
        keyId = j.value("keyId", 0);
    }
};