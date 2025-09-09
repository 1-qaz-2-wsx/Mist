#include "ItemDatabase.h"
#include "json.hpp" // 包含你的JSON库头文件
#include <fstream>   // 用于文件读写
#include <iostream>  // 用于输出错误信息

// 为了方便使用，我们为json库创建一个命名空间别名
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
        newItem.fromJson(itemObject); // 直接让 Item 对象自己从 JSON 加载！

        // 使用从 item 中获取的 id 作为 key
        itemTemplates_[newItem.getId()] = newItem;
    }

    std::cout << "Loaded " << itemTemplates_.size() << " items from " << filename << std::endl;
    return true;
}

const Item* ItemDatabase::getItemTemplate(unsigned int itemId) const {
    // 使用.find()来安全地查找元素
    auto it = itemTemplates_.find(itemId);

    if (it != itemTemplates_.end()) {
        // 如果找到了，it->second 就是我们想要的 Item 对象
        return &(it->second);
    }

    // 如果没找到，返回空指针
    return nullptr;
}