#include "ItemDatabase.h"
#include "json.hpp" // �������JSON��ͷ�ļ�
#include <fstream>   // �����ļ���д
#include <iostream>  // �������������Ϣ

// Ϊ�˷���ʹ�ã�����Ϊjson�ⴴ��һ�������ռ����
using json = nlohmann::json;

bool ItemDatabase::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open item database file: " << filename << std::endl;
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

    itemTemplates_.clear();

    for (const auto& itemObject : jsonData) {
        Item newItem;
        newItem.fromJson(itemObject); // ֱ���� Item �����Լ��� JSON ���أ�

        // ʹ�ô� item �л�ȡ�� id ��Ϊ key
        itemTemplates_[newItem.getId()] = newItem;
    }

    std::cout << "Loaded " << itemTemplates_.size() << " items from " << filename << std::endl;
    return true;
}

const Item* ItemDatabase::getItemTemplate(unsigned int itemId) const {
    // ʹ��.find()����ȫ�ز���Ԫ��
    auto it = itemTemplates_.find(itemId);

    if (it != itemTemplates_.end()) {
        // ����ҵ��ˣ�it->second ����������Ҫ�� Item ����
        return &(it->second);
    }

    // ���û�ҵ������ؿ�ָ��
    return nullptr;
}