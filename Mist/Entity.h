#pragma once
#include <string> 
#include "json.hpp"

using json = nlohmann::json;

class Entity
{
protected:
	std::string name_;  //名称
	unsigned int id_;  //唯一ID
public:
	Entity();
	Entity(const std::string& name, unsigned int id);

	const std::string& getName()const;
	unsigned int getId() const;


	// JSON 读写
	virtual void toJson(json& j) const; //转换为 JSON 对象
	virtual void fromJson(const json& j); //从 JSON 对象加载

	//析构函数
	virtual ~Entity() {};

};
