#include "Entity.h"
#include "Entity.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

Entity::Entity() : name(""), id_(0) {}
Entity::Entity(const string& name, unsigned int id) : name(name), id_(id) {}

//访问器
const string& Entity::getName() const { return name; }

unsigned int Entity::getId() const { return id_; }

//返回小写名称
string Entity::CompName() const
{
    string lower_name = name;
    transform(lower_name.begin(), lower_name.end(), lower_name.begin(),
        [](unsigned char c) { return tolower(c); });
    return lower_name;
}

//完全匹配
bool Entity::FullMatch(const std::string& name) const {
    return CompName() == name;
}

//部分匹配
bool Entity::Match(const std::string& name) const {
    std::string name = CompName();     // 当前对象的名字（小写）
    std::string search = name;

    // 转成小写（避免大小写干扰）
    std::transform(search.begin(), search.end(), search.begin(),
        [](unsigned char c) { return std::tolower(c); });

    size_t pos = name.find(search);     // 查找子串
    while (pos != std::string::npos) {
        // 匹配必须在开头，或是一个单词边界（前一个字符是空格）
        if (pos == 0 || name[pos - 1] == ' ')
            return true;
        pos = name.find(search, pos + 1);   // 继续查找下一个
    }
    return false;
}

// JSON 读写
void Entity::toJson(json& j) const {
	j = json{
		{"name", name},
		{"id", id_}
	};
}

void Entity::fromJson(const json& j) {
	if (j.contains("name") && j["name"].is_string()) {
		name = j["name"];
	}
	if (j.contains("id") && j["id"].is_number_unsigned()) {
		id_ = j["id"];
	}
}
