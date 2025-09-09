#pragma once

#include <string>
#include <unordered_map>
#include "Item.h" // 包含物品的定义

class ItemDatabase {
public:
    /**
     * @brief 从指定的JSON文件加载所有物品模板
     * @param filename JSON文件的路径
     * @return 如果加载成功返回 true, 否则返回 false
     */
    bool load(const std::string& filename);

    /**
     * @brief 根据物品ID获取一个指向物品模板的常量指针
     * @param itemId 物品的唯一ID
     * @return 如果找到则返回物品模板指针，否则返回 nullptr
     */
    const Item* getItemTemplate(unsigned int itemId) const;

    // 在 ItemDatabase.h/.cpp 中添加:
    std::unique_ptr<Item> createInstance(unsigned int itemId) const;

private:
    // 使用哈希表(unordered_map)来存储物品模板，通过ID可以快速查找
    // 键是 物品ID(unsigned int)，值是 物品对象(Item)
    std::unordered_map<unsigned int, Item> itemTemplates_;
};