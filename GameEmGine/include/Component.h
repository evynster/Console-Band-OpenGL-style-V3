#pragma once
#include <string>
#include <unordered_map>
class Component
{
protected:
	typedef std::string COMP_TYPE;
	Component():m_type("UNKNOWN"){ m_compList[m_type]++; }
	Component(COMP_TYPE type):m_type(type) { m_compList[type]++;/*this dose not matter but I did it anyways*/ };
	COMP_TYPE m_type;

public:
	Component::COMP_TYPE getType()
	{
		return m_type;
	}
	static const std::unordered_map<COMP_TYPE, unsigned>& getComponentList()
	{
		return m_compList;
	}

private:
	static std::unordered_map<COMP_TYPE, unsigned> m_compList;
};

