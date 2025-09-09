#include "Item.h"

void Item::fromJson(const json& j) {
    // 1. ���û���(Entity)�� fromJson ����������ͨ������ (id �� name)
    // ����ʵ�ּ̳еĹؼ�һ�����������ظ�����
    Entity::fromJson(j);

    // 2. ���� Item ���Լ����е�����
    description = j.at("description").get<std::string>();

    // ���δ�� Item �и������� (���繥����)��Ҳ���������
    // attackDamage = j.value("attack", 0); // .value() �����ṩĬ��ֵ������ȫ
}

void Item::toJson(json& j) const {
    // ͬ�����ȵ��û���� toJson ����
    Entity::toJson(j);
    // ������Լ�������
    j["description"] = description;
}