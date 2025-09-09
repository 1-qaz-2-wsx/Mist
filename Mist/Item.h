#pragma once

#include "Entity.h" // ��������ͷ�ļ�
#include <string>

// ʹ�����Ѿ��� Entity.h �ж���� json ����
using json = nlohmann::json;

class Item : public Entity {
public:
    std::string description; // ��Ʒ���е�����

public:
    // Ĭ�Ϲ��캯��
    Item() = default;

    // һ������Ĺ��캯��
    Item(unsigned int id, const std::string& name, const std::string& desc)
        : Entity(name, id), description(desc) {}

    // --- ��д(override)������麯�� ---
    // ���ǽ�������ʵ�־����JSON�����߼�

    /**
     * @brief ��JSON����������������Item����ĳ�Ա
     * @param j nlohmann::json ����
     */
    virtual void fromJson(const json& j) override;

    /**
     * @brief ��Item���������ת��(���л�)ΪJSON����
     * @param j nlohmann::json ����
     */
    virtual void toJson(json& j) const override;
};