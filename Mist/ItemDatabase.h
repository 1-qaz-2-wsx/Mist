#pragma once

#include <string>
#include <unordered_map>
#include "Item.h" // ������Ʒ�Ķ���

class ItemDatabase {
public:
    /**
     * @brief ��ָ����JSON�ļ�����������Ʒģ��
     * @param filename JSON�ļ���·��
     * @return ������سɹ����� true, ���򷵻� false
     */
    bool load(const std::string& filename);

    /**
     * @brief ������ƷID��ȡһ��ָ����Ʒģ��ĳ���ָ��
     * @param itemId ��Ʒ��ΨһID
     * @return ����ҵ��򷵻���Ʒģ��ָ�룬���򷵻� nullptr
     */
    const Item* getItemTemplate(unsigned int itemId) const;

private:
    // ʹ�ù�ϣ��(unordered_map)���洢��Ʒģ�壬ͨ��ID���Կ��ٲ���
    // ���� ��ƷID(unsigned int)��ֵ�� ��Ʒ����(Item)
    std::unordered_map<unsigned int, Item> itemTemplates_;
};