#include "Component.h"
std::unordered_map<Component::COMP_TYPE, unsigned> Component::m_compList;

void Component::addChild(Component* child)
{
	if(!child)return;

	if(std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
	child->m_parent = this;
}

void Component::removeChild(Component* child)
{
	if(!child)return;

	child->m_parent = nullptr;

	std::vector<Component*>::iterator ref;
	if((ref = std::find(m_children.begin(), m_children.end(), child)) != m_children.end())
		m_children.erase(ref);


}

void Component::removeChild(unsigned index)
{
	if(index >= m_children.size())return;

	m_children[index]->m_parent = nullptr;
	m_children.erase(m_children.begin() + index);

}

void Component::setParent(Component* parent)
{
	if(!parent)return;

	parent->addChild(this);

}

void Component::removeParent(Component* parent)
{
	if(!parent)return;

	parent->removeChild(this);
}

Component* Component::getChild(unsigned int index)
{
	return m_children[index];
}

Component* Component::getParent()
{
	return m_parent;
}

std::vector<Component*>& Component::getChildren()
{
	return m_children;
}

