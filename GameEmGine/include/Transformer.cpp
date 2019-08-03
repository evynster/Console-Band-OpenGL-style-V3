#include "Transformer.h"


Transformer::Transformer():m_translate(1), m_rotate(1), m_scale(1), m_scaleDat(1)
{}

Transformer::~Transformer()
{}

void Transformer::reset()
{
	m_translate = m_rotate = m_scale = glm::mat4(1);
	m_posDat = m_rotDat = m_scaleDat = {0,0,0};
}

void Transformer::enableFPS(bool enable)
{
	m_fps = enable;
	if(!enable)
	{
		m_forward = {0,0,1};
		m_up = {0,1,0};
		m_right = {1,0,0};
	}
}

void Transformer::rotate(Coord3D<> angles)
{
	m_updatedRot = true;
	m_rotate = glm::mat4(1);


	m_rotDat.z = fmodf(angles.z, 360.f);
	m_rotDat.y = fmodf(angles.y, 360.f);
	m_rotDat.x = fmodf(angles.x, 360.f);

	if(angles.z)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.z), Coord3D<>{0, 0, 1});

	if(angles.y)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.y), Coord3D<>{0, 1, 0});

	if(angles.x)
		m_rotate *= Quat::quatRotationMat(glm::radians(m_rotDat.x), Coord3D<>{1, 0, 0});

	if(m_fps)
	{
		m_forward = reclass(Coord3D<>, m_rotate * glm::vec4(0, 0, 1, 1));
		m_up = reclass(Coord3D<>, m_rotate * glm::vec4(0, 1, 0, 1));
		m_right = reclass(Coord3D<>, m_rotate * glm::vec4(1, 0, 0, 1));
	}

	m_forward.normalize();
	m_up.normalize();
	m_right.normalize();
	
	if(m_parent)
		[&]() {
		Transformer* parent = m_parent;
		while(parent)
			m_rotate = m_parent->m_rotate * m_rotate,
			parent = m_parent->m_parent;
	}();
}

void Transformer::rotateBy(Coord3D<> angles)
{
	m_updatedRot = true;
	rotate(angles + m_rotDat);
}

void Transformer::translateBy(float x, float y, float z)
{
	translateBy( Coord3D<>{x, y, z});
}

void Transformer::translateBy(Coord3D<> pos)
{
	m_updatedTrans = true;
	m_translate = glm::translate(m_translate, reclass(glm::vec3, ((m_forward * -pos.z) + (m_up * pos.y) + (m_right * pos.x))));
	m_posDat += ((m_forward * -pos.z) + (m_up * pos.y) + (m_right * pos.x));

	if(m_parent)
		[&]() {
		Transformer* parent = m_parent;
		while(parent)
			m_translate = m_parent->m_translate * m_translate,
			parent = m_parent->m_parent;
	}();
}

void Transformer::translate(float x, float y, float z)
{
	translate(Coord3D<>{x, y, z});
}

void Transformer::translate(Coord3D<> pos)
{
	m_updatedTrans = true;
	m_posDat = pos;
	m_translate = glm::translate(glm::mat4(1), reclass(glm::vec3, ((m_forward * -m_posDat.z) + (m_up * m_posDat.y) + (m_right * m_posDat.x))));

	if(m_parent)
		[&]() {
		Transformer* parent = m_parent;
		while(parent)
			m_translate = m_parent->m_translate * m_translate,
			parent = m_parent->m_parent;
	}();
}

void Transformer::scaleBy(float scale)
{
	scaleBy(scale, scale, scale);
}

void Transformer::scaleBy(float x, float y, float z)
{
	setScale(m_scaleDat + Coord3D<>(x, y, z));
}

void Transformer::setScale(float scale)
{
	setScale(scale, scale, scale);
}

void Transformer::setScale(Coord3D<> scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void Transformer::setScale(float x, float y, float z)
{
	m_updatedScale = true;
	m_scaleDat = Coord3D<>(x, y, z);
	m_scale = glm::scale(glm::mat4(1), *(glm::vec3*) & m_scaleDat);

	if(m_parent)
		[&]() {
		Transformer* parent = m_parent;
		while(parent)
			m_scale = m_parent->m_scale * m_scale,
			parent = m_parent->m_parent;
	}();
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

glm::mat4& Transformer::getRotationMatrix()
{
	//	m_updatedRot = false;
	return m_rotate;
}

glm::mat4& Transformer::getScaleMatrix()
{
	//	m_updatedScale = false;
	return m_scale;
}

glm::mat4& Transformer::getTranslationMatrix()
{
	//	m_updatedTrans = false;
	return m_translate;
}

glm::mat4 Transformer::getTransformation()
{
	//	m_updatedRot = m_updatedTrans
	//		= m_updatedScale = false;
	return   m_translate * m_rotate * m_scale;
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
	m_children.push_back(child);
	child->m_parent = this;
}


void Transformer::removeChild(Transformer* child)
{
	if(child)
	{
		std::vector<Transformer*>::iterator ref;
		while(( ref = std::find(m_children.begin(), m_children.end(), child) ) != m_children.end())
			m_children.erase(ref);
		
	}
}

Transformer* Transformer::getChild(uint index)
{
	return m_children[index];
}

std::vector<Transformer*>& Transformer::getChildren()
{
	return m_children;
}

CLASS_TYPE Transformer::getType()
{
	return m_type;
}
