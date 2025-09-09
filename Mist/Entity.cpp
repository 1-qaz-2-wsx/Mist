#include "Entity.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

// 构造函数
Entity::Entity() : name_(""), id_(0) {}
Entity::Entity(const std::string& name, unsigned int id) : name_(name), id_(id) {}

// 访问器
const std::string& Entity::getName() const { return name_; }

unsigned int Entity::getId() const { return id_; }

// JSON 读写
void Entity::toJson(json& j) const {
	j = json{
		{"name", name_},
		{"id", id_}
	};
}

void Entity::fromJson(const json& j) {
	if (j.contains("name") && j["name"].is_string()) {
		name_ = j["name"];
	}
	if (j.contains("id") && j["id"].is_number_unsigned()) {
		id_ = j["id"];
	}
}