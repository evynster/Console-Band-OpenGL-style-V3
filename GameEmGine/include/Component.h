#pragma once
#include <string>
#include <unordered_map>
class Component
{
public:
//enum COMP_TYPE{UNKNOWN};
	typedef std::string COMP_TYPE;
private:
	static std::unordered_map<COMP_TYPE, unsigned> m_compList;
	std::vector<Component* >m_children;
	Component* m_parent;
protected:
	Component(Component*parent=nullptr) :m_parent(parent){ m_type="UNKNOWN"; m_compList[m_type]++; }
	Component(COMP_TYPE type, Component* parent = nullptr):m_parent(parent) { m_type=type; m_compList[m_type]++;/*this dose not matter but I did it anyways*/ };
	virtual ~Component() { --m_compList[m_type]; if(!m_compList[m_type]) m_compList.erase(m_type); };
	COMP_TYPE m_type;

public:
virtual	COMP_TYPE getCompType()
	{
		return m_type;
	}
	static const std::unordered_map<COMP_TYPE, unsigned>& getComponentList()
	{
		return m_compList;
	}

	virtual void addChild(Component* child);
	virtual void removeChild(Component* child);
	virtual void removeChild(unsigned index);
	virtual void setParent(Component* parent);
	virtual void removeParent(Component* parent);

	virtual Component* getChild(unsigned int index);
	virtual Component* getParent();
	virtual std::vector<Component*>& getChildren();
};

