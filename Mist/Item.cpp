#include "Item.h"

void Item::fromJson(const json& j) {
    // 1. 调用基类(Entity)的 fromJson 函数来加载通用数据 (id 和 name)
    // 这是实现继承的关键一步，避免了重复代码
    Entity::fromJson(j);

    // 2. 加载 Item 类自己特有的数据
    description = j.at("description").get<std::string>();

    // 如果未来 Item 有更多属性 (比如攻击力)，也在这里加载
    // attackDamage = j.value("attack", 0); // .value() 可以提供默认值，更安全
}

void Item::toJson(json& j) const {
    // 同样，先调用基类的 toJson 函数
    Entity::toJson(j);
    // 再添加自己的数据
    j["description"] = description;
}