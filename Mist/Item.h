#pragma once

#include "Entity.h" // 包含基类头文件
#include <string>

// 使用你已经在 Entity.h 中定义的 json 别名
using json = nlohmann::json;

class Item : public Entity {
public:
    std::string description; // 物品特有的属性

public:
    // 默认构造函数
    Item() = default;

    // 一个方便的构造函数
    Item(unsigned int id, const std::string& name, const std::string& desc)
        : Entity(name, id), description(desc) {}

    // --- 重写(override)基类的虚函数 ---
    // 我们将在这里实现具体的JSON加载逻辑

    /**
     * @brief 从JSON对象加载数据来填充Item对象的成员
     * @param j nlohmann::json 对象
     */
    virtual void fromJson(const json& j) override;

    /**
     * @brief 将Item对象的数据转换(序列化)为JSON对象
     * @param j nlohmann::json 对象
     */
    virtual void toJson(json& j) const override;
};