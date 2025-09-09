#pragma once
#include <string> 
#include "json.hpp"

using json = nlohmann::json;

class Entity
{
protected:
	std::string name;  //����
	unsigned int id;  //ΨһID
public:
	Entity();
	Entity(const std::string& name, unsigned int id);

	const std::string& getName()const;
	unsigned int getId() const;

	//�ȽϺ���
	std::string CompName() const; //����Сд����

	bool FullMatch(const std::string& name) const; //��ȫƥ��
	bool Match(const std::string& name) const; //����ƥ��

	// JSON ��д
	virtual void toJson(json& j) const; //ת��Ϊ JSON ����
	virtual void fromJson(const json& j); //�� JSON �������

	//��������
	virtual ~Entity() {};

};
