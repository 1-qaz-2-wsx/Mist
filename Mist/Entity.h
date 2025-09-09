#pragma once
#include <string> 
#include "json.hpp"

using json = nlohmann::json;

class Entity
{
protected:
	std::string name;  //名称
	unsigned int id;  //唯一ID
public:
	Entity();
	Entity(const std::string& name, unsigned int id);

	const std::string& getName()const;
	unsigned int getId() const;

	//比较函数
	std::string CompName() const; //返回小写名称

	bool FullMatch(const std::string& name) const; //完全匹配
	bool Match(const std::string& name) const; //部分匹配

	// JSON 读写
	virtual void toJson(json& j) const; //转换为 JSON 对象
	virtual void fromJson(const json& j); //从 JSON 对象加载

	//析构函数
	virtual ~Entity() {};

};
