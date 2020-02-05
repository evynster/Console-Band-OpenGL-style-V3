#include "Transformer.h"


Transformer::Transformer():
	m_localTranslate(1), m_localRotate(1),
	m_localScale(1), m_worldTranslate(1),
	m_worldRotate(1), m_worldScale(1),
	m_fps(false), m_parent(nullptr),
	m_type(Transformer::TYPE::TRANSFORMER)
{}

Transformer::~Transformer()
{}

void Transformer::reset()
{
	m_localTranslate = m_localRotate = m_localScale = glm::mat4(1);
	m_posDat = m_rotDat = {0,0,0};
	m_scaleDat = {1,1,1};

	m_forward = {0,0,1};
	m_up = {0,1,0};
	m_right = {1,0,0};



}

void Transformer::enableFPSMode(bool enable)
{
	m_fps = enable;
}

void Transformer::rotate(Coord3D<> angles)
{
	m_updatedRot = true;
	m_localRotate = glm::mat4(1);


	m_rotDat.z = fmodf(angles.z, 360.f);
	m_rotDat.y = fmodf(angles.y, 360.f);
	m_rotDat.x = fmodf(angles.x, 360.f);

	if(angles.z)
		m_localRotate *= Quat::quatRotationMat(m_rotDat.z, Coord3D<>{0, 0, 1});

	if(angles.y)
		m_localRotate *= Quat::quatRotationMat(m_rotDat.y, Coord3D<>{0, 1, 0});

	if(angles.x)
		m_localRotate *= Quat::quatRotationMat(m_rotDat.x, Coord3D<>{1, 0, 0});

	m_forward = reclass(Coord3D<>, m_localRotate * glm::vec4(0, 0, 1, 1));
	m_up = reclass(Coord3D<>, m_localRotate * glm::vec4(0, 1, 0, 1));
	m_right = reclass(Coord3D<>, m_localRotate * glm::vec4(1, 0, 0, 1));

	m_forward.normalize();
	m_up.normalize();
	m_right.normalize();


}

void Transformer::rotate(float x, float y, float z)
{
	Transformer::rotate({x,y,z});
}

void Transformer::rotateBy(Coord3D<> angles)
{
	Transformer::rotate(angles + m_rotDat);
}

void Transformer::rotateBy(float x, float y, float z)
{
	Transformer::rotateBy({x,y,z});
}

void Transformer::translateBy(float x, float y, float z)
{
	Transformer::translateBy({x, y, z});
}

void Transformer::translateBy(Coord3D<> pos)
{
	m_updatedTrans = true;

	auto forward = m_forward;
	auto up = m_up;
	auto right = m_right;

	if(!m_fps)
		forward = {0,0,1},
		up = {0,1,0},
		right = {1,0,0};

	m_posDat += ((forward * -pos.z) + (up * pos.y) + (right * pos.x));
	m_localTranslate = glm::translate(glm::mat4(1), reclass(glm::vec3, m_posDat));



}

void Transformer::translate(float x, float y, float z)
{
	Transformer::translate(Coord3D<>{x, y, z});
}

void Transformer::translate(Coord3D<> pos)
{
	m_updatedTrans = true;

	auto forward = m_forward;
	auto up = m_up;
	auto right = m_right;

	if(!m_fps)
		forward = {0,0,1},
		up = {0,1,0},
		right = {1,0,0};

	m_posDat = pos.x * right + pos.y * up + -pos.z * forward;
	m_localTranslate = glm::translate(glm::mat4(1), reclass(glm::vec3, m_posDat));
}

void Transformer::scaleBy(float scale)
{
	Transformer::scaleBy(m_scaleDat.x + scale, m_scaleDat.y + scale, m_scaleDat.z + scale);
}

void Transformer::scaleBy(float x, float y, float z)
{
	Transformer::setScale(m_scaleDat + Coord3D<>(x, y, z));
}

void Transformer::setScale(float scale)
{
	Transformer::setScale(scale, scale, scale);
}

void Transformer::setScale(Coord3D<> scale)
{
	Transformer::setScale(scale.x, scale.y, scale.z);
}

void Transformer::setScale(float x, float y, float z)
{
	m_updatedScale = true;

	auto forward = m_forward;
	auto up = m_up;
	auto right = m_right;

	if(!m_fps)
		forward = {0,0,1},
		up = {0,1,0},
		right = {1,0,0};

	m_scaleDat = x * right + y * up + z * forward;
	m_localScale = glm::scale(glm::mat4(1), reclass(glm::vec3, m_scaleDat));
}

Coord3D<> Transformer::getPosition()
{
	return m_posDat;
}

Coord3D<> Transformer::getRotation()
{
	return m_rotDat;
}

Coord3D<> Transformer::getScale()
{
	return m_scaleDat;
}

Coord3D<> Transformer::getForward()
{

	return m_fps ? Coord3D<>{0, 0, 1} : m_forward;
}

Coord3D<> Transformer::getUp()
{
	return m_fps ? Coord3D<>{0, 1, 0} : m_up;
}

Coord3D<> Transformer::getRight()
{
	return m_fps ? Coord3D<>{1, 0, 0} : m_right;
}

glm::mat4 Transformer::getLocalRotationMatrix()
{
	return m_localRotate;
}

glm::mat4 Transformer::getLocalScaleMatrix()
{
	return m_localScale;
}

glm::mat4 Transformer::getLocalTranslationMatrix()
{
	return m_localTranslate;
}

glm::mat4 Transformer::getWorldRotationMatrix()
{
	calculateWorldRotationMatrix();
	return m_worldRotate;
}

glm::mat4 Transformer::getWorldScaleMatrix()
{
	calculateWorldScaleMatrix();
	return m_worldScale;
}

glm::mat4 Transformer::getWorldTranslationMatrix()
{
	calculateWorldTranslationMatrix();
	return m_worldTranslate;
}

void Transformer::calculateWorldRotationMatrix()
{
	for(auto& a : m_children)
		a->m_updatedRot = true;

	m_worldRotate = glm::mat4(1);
	static Transformer* parent; parent = m_parent;

	if(m_parent)
		while(parent)
			m_worldRotate = parent->m_localRotate * m_worldRotate,
			parent = parent->m_parent;

}

void Transformer::calculateWorldScaleMatrix()
{
	for(auto& a : m_children)
		a->m_updatedScale = true;

	m_worldScale = glm::mat4(1);
	static Transformer* parent; parent = m_parent;
	if(m_parent)
		while(parent)
			m_worldScale = parent->m_localScale * m_worldScale,
			parent = parent->m_parent;
}

void Transformer::calculateWorldTranslationMatrix()
{
	for(auto& a : m_children)
		a->m_updatedTrans = true;

	m_worldTranslate = glm::mat4(1);
	static Transformer* parent; parent = m_parent;

	if(m_parent)
		while(parent)
			m_worldTranslate = parent->m_localTranslate * m_worldTranslate,
			parent = parent->m_parent;

}

glm::mat4 Transformer::getLocalTransformation()
{
	return getLocalTranslationMatrix() * getLocalRotationMatrix() * getLocalScaleMatrix();
}

glm::mat4 Transformer::getWorldTransformation()
{
	return getWorldTranslationMatrix() * getWorldRotationMatrix() * getWorldScaleMatrix();
}

void Transformer::resetUpdated()
{
	m_updatedRot = m_updatedTrans
		= m_updatedScale = false;
}

bool Transformer::isUpdated()
{
	return m_updatedRot || m_updatedTrans || m_updatedScale;
}

bool Transformer::isScaleUpdated()
{
	return m_updatedScale;
}

bool Transformer::isRotationUpdated()
{
	return m_updatedRot;
}

bool Transformer::isTranslatinUpdated()
{
	return m_updatedTrans;
}

void Transformer::addChild(Transformer* child)
{
	if(!child)return;

	if(std::find(m_children.begin(), m_children.end(), child) == m_children.end())
		m_children.push_back(child);
	child->m_parent = this;
}

void Transformer::removeChild(Transformer* child)
{
	if(!child)return;

	child->m_parent = nullptr;

	std::vector<Transformer*>::iterator ref;
	if((ref = std::find(m_children.begin(), m_children.end(), child)) != m_children.end())
		m_children.erase(ref);


}

void Transformer::removeChild(unsigned index)
{
	if(index >= m_children.size())return;

	m_children[index]->m_parent = nullptr;
	m_children.erase(m_children.begin() + index);

}

void Transformer::setParent(Transformer* parent)
{
	if(!parent)return;

	parent->addChild(this);

}

void Transformer::removeParent(Transformer* parent)
{
	if(!parent)return;

	parent->removeChild(this);
}

Transformer* Transformer::getChild(unsigned int index)
{
	return m_children[index];
}

Transformer* Transformer::getParent()
{
	return m_parent;
}

std::vector<Transformer*>& Transformer::getChildren()
{
	return m_children;
}

Transformer::TYPE Transformer::getType()
{
	return m_type;
}
