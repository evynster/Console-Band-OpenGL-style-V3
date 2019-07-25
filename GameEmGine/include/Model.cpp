#include "Model.h" 
#include "ResourceManager.h"
#include <ctime>

Model::Model()
{}

Model::Model(Model& model):
	m_transform(model.m_transform),
	m_mesh(model.m_mesh),
	m_colour(model.m_colour),
	m_transBB(glm::mat4(1)),
	m_render(model.m_render)
{
	m_shaderBB = ResourceManager::getShader("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");
	//boundingBoxInit();
}

Model::Model(const char* path):
	m_transBB(glm::mat4(1))
{
	if(loadModel(path))
	{
		m_shaderBB = ResourceManager::getShader("Shaders/BoundingBox.vtsh", "Shaders/BoundingBox.fmsh");
		m_left = m_mesh.left;
		m_right = m_mesh.right;
		m_top = m_mesh.top;
		m_bottom = m_mesh.bottom;
		m_front = m_mesh.front;
		m_back = m_mesh.back;
		boundingBoxUpdate();
	}
}

Model::~Model()
{}

/// - Collision Function - ///

bool Model::collision2D(Model* k)
{
	//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
	Coord3D thing = getCenter() - k->getCenter();

	float distanceX = abs(thing.x);
	float distanceZ = abs(thing.z);

	float capW = (getWidth() + k->getWidth()) / 2;
	float capD = (getDepth() + k->getDepth()) / 2;

	if(std::abs(distanceX) <= capW)
		if(std::abs(distanceZ) <= capD)
			return true;

	return false;
}

bool Model::collision2D(Model* l, Model* k)
{
	//if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
	Coord3D thing = l->getCenter() - k->getCenter();

	float distanceX = abs(thing.x);
	float distanceZ = abs(thing.z);

	float capW = (l->getWidth() + k->getWidth()) / 2;
	float capD = (l->getDepth() + k->getDepth()) / 2;

	if(std::abs(distanceX) <= capW)
		if(std::abs(distanceZ) <= capD)
			return true;

	return false;
}


///~ 3D Collision Function ~///

bool Model::collision3D(Model* k)
{
	return collision3D(this, k);
}

float dotProduct(Coord3D a, Coord3D b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool Model::collision3D(Model* box1, Model* box2)
{
	////if distance between mod in the x OR z is less than half of both widths combined then collide and don't allow any more movement in that direction.
	//Coord3D thing = l->getCenter() - k->getCenter();
	//
	//float distanceX = abs(thing.x);
	//float distanceY = abs(thing.y);
	//float distanceZ = abs(thing.z);
	//
	//float capW = (l->getWidth() + k->getWidth()) / 2;
	//float capH = (l->getHeight() + k->getHeight()) / 2;
	//float capD = (l->getDepth() + k->getDepth()) / 2;
	//
	//if(std::abs(distanceX) <= abs(capW))
	//	if(std::abs(distanceZ) <= abs(capD))
	//		if(std::abs(distanceY) <= abs(capH))
	//			return true;
	//
	//return false;
	static Coord3D RPos = box1->getTransformer().getPosition() - box2->getTransformer().getPosition();
	Coord3D AxisX{1,0,0}, AxisY{0,1,0}, AxisZ{0,0,1};

	glm::mat4* trans1 = &box1->m_transform.getRotationMatrix(), * trans2 = &box2->m_transform.getRotationMatrix();

	Coord3D AxisX1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisX.x, AxisX.y, AxisX.z, 1));
	Coord3D AxisY1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisY.x, AxisY.y, AxisY.z, 1));
	Coord3D AxisZ1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisZ.x, AxisZ.y, AxisZ.z, 1));

	Coord3D AxisX2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisX.x, AxisX.y, AxisX.z, 1));
	Coord3D AxisY2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisY.x, AxisY.y, AxisY.z, 1));
	Coord3D AxisZ2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisZ.x, AxisZ.y, AxisZ.z, 1));

	return !(
		getSeparatingPlane(RPos, AxisX1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ1, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisX2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisY2, *box1, *box2) ||
		getSeparatingPlane(RPos, AxisZ2, *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisX1 , AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisX1 , AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisX1 , AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisY1 , AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisY1 , AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisY1 , AxisZ2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisZ1 , AxisX2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisZ1 , AxisY2), *box1, *box2) ||
		getSeparatingPlane(RPos, dotProduct(AxisZ1 , AxisZ2), *box1, *box2));

}


Coord3D crossProduct(Coord3D a, Coord3D b)
{
	return 
	{
		a.y * b.z - a.z * b.y, 
		a.z * b.y - a.y * b.z, 
		a.x * b.y - a.y * b.x
	};
}

bool Model::getSeparatingPlane(const Coord3D& RPos, const Coord3D& Plane, Model& box1, Model& box2)
{
	Coord3D AxisX{1,0,0}, AxisY{0,1,0}, AxisZ{0,0,1};

	glm::mat4* trans1 = &box1.m_transform.getRotationMatrix(), * trans2 = &box2.m_transform.getRotationMatrix();

	Coord3D AxisX1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisX.x, AxisX.y, AxisX.z, 1));
	Coord3D AxisY1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisY.x, AxisY.y, AxisY.z, 1));
	Coord3D AxisZ1 = *(Coord3D*) & (*trans1 * glm::vec4(AxisZ.x, AxisZ.y, AxisZ.z, 1));

	Coord3D AxisX2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisX.x, AxisX.y, AxisX.z, 1));
	Coord3D AxisY2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisY.x, AxisY.y, AxisY.z, 1));
	Coord3D AxisZ2 = *(Coord3D*) & (*trans2 * glm::vec4(AxisZ.x, AxisZ.y, AxisZ.z, 1));



	return (fabs(dotProduct(RPos, Plane)) >
		(
			fabs(dotProduct((AxisX1 * box1.m_width / 2), Plane)) +
			fabs(dotProduct((AxisY1 * box1.m_height / 2), Plane)) +
			fabs(dotProduct((AxisZ1 * box1.m_depth / 2), Plane)) +

			fabs(dotProduct((AxisX2 * box2.m_width / 2), Plane)) +
			fabs(dotProduct((AxisY2 * box2.m_height / 2), Plane)) +
			fabs(dotProduct((AxisZ2 * box2.m_depth / 2), Plane))));
}


void Model::render(Shader& shader, glm::mat4& cam)
{
	float colour[4]{(float)m_colour.colorR / 255,(float)m_colour.colorG / 255,(float)m_colour.colorB / 255,(float)m_colour.colorA / 255};

	shader.enable();
	m_shader = &shader;

	if(m_parent)
		glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &(([&]()->glm::mat4
			{
				glm::mat4 tmp(1);
				Model* parent = m_parent;
				while(parent)
				{
					tmp = parent->getTransformer().getTransformation() * tmp;
					parent = parent->m_parent;
				}
				return tmp;
			}() * m_transform.getTransformation())[0][0]));
	else
		glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &((m_transform.getTransformation())[0][0]));

	glUniform4fv(shader.getUniformLocation("colourMod"), 1, colour);

	if(m_animations[m_animation])
		m_animations[m_animation]->update(&shader, &m_mesh);

	// update the position of the object
	m_transBB = cam * (m_transform.getTranslationMatrix());
	boundingBoxUpdate();

	if(m_render)
	{
		//render the mesh
		m_mesh.render(shader);

		if(m_enableBB)
			drawBoundingBox();

		m_transform.resetUpdated();

		for(auto& a : m_children)
			a->render(shader, cam);
	}
	shader.disable();
}

void Model::drawBoundingBox()
{

	m_shaderBB->enable();
	float colour[4]{(float)m_colour.colorR / 255,(float)m_colour.colorG / 255,(float)m_colour.colorB / 255,(float)m_colour.colorA / 255};

	glUniform4fv(m_shaderBB->getUniformLocation("colourMod"), 1, colour);

	glBindVertexArray(m_BBVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);

	m_shaderBB->disable();
}

Transformer& Model::getTransformer()
{
	return m_transform;
}

void Model::removeChild(Model* child)
{
	if(child)
	{
		auto ref = std::find(m_children.begin(), m_children.end(), child);
		if(ref != m_children.end())
			m_children.erase(ref);
	}
}

void Model::addChild(Model* child)
{
	m_children.push_back(child);
	m_children.back()->m_parent = this;
	//m_transform.addChild(&child->m_transform);
}

void Model::setColour(float r, float g, float b, float a)
{
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b), (GLubyte)(255 * a));
}

void Model::setColour(float r, float g, float b)
{
	m_colour.set((GLubyte)(255 * r), (GLubyte)(255 * g), (GLubyte)(255 * b));
}

void Model::setColour(ColourRGBA colour)
{
	m_colour = colour;
}

ColourRGBA Model::getColour()
{
	return m_colour;
}

bool Model::loadModel(const char* path)
{
	return m_mesh.loadMesh(path);
}

void Model::enableBoundingBox(bool enable)
{
	m_enableBB = enable;
}

void Model::addAnimation(std::string tag, Animation* animation)
{
	m_animations[tag] = animation;
}

//void Model::addFrameBuffer(FrameBuffer * buffer)
//{
//	m_frameBuffers[buffer->getTag()] = buffer;
//}
//
//void Model::removeFrameBuffer(std::string tag)
//{
//	m_frameBuffers.erase(tag);
//}
//
//std::unordered_map<std::string, FrameBuffer*>& Model::getFrameBuffers()
//{
//	return m_frameBuffers;
//}

float Model::getWidth()
{
	return m_width;
}

float Model::getHeight()
{
	return m_height;
}

float Model::getDepth()
{

	return m_depth;
}

Coord3D Model::getCenter()
{

	return m_center;
}

void Model::boundingBoxUpdate()
{

	m_front = m_back = m_top = m_bottom = m_left = m_right = Coord3D(0, 0, 0);
	std::vector<glm::vec4> thing
	{
	glm::vec4(*(glm::vec3*) & m_mesh.right, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.left, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.top, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.bottom, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.front, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.back, 1)
	};

	//bool first = true;

	if(m_enableBB)
	{
		m_shaderBB->enable();
		glUniformMatrix4fv(m_shaderBB->getUniformLocation("trans"), 1, false, &(m_transBB)[0][0]);
		m_shaderBB->disable();
	}

	glm::mat4 tmpMat = m_parent ? m_parent->m_transform.getScaleMatrix() *
		m_transform.getScaleMatrix() : m_transform.getScaleMatrix();
	for(auto& a : thing)
		a = (tmpMat * a);

	m_right = *(Coord3D*)& thing[0];
	m_left = *(Coord3D*)& thing[1];
	m_top = *(Coord3D*)& thing[2];
	m_bottom = *(Coord3D*)& thing[3];
	m_front = *(Coord3D*)& thing[4];
	m_back = *(Coord3D*)& thing[5];

	m_width = abs(m_right.x - m_left.x);
	m_height = abs(m_top.y - m_bottom.y);
	m_depth = abs(m_front.z - m_back.z);
	m_center = Coord3D((m_right.x + m_left.x), (m_top.y + m_bottom.y), (m_front.z + m_back.z)) / 2;

	thing.clear();//for assurance 
	thing =
	{
	glm::vec4(*(glm::vec3*) & m_mesh.right, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.left, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.top, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.bottom, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.front, 1),
	glm::vec4(*(glm::vec3*) & m_mesh.back, 1)
	};


	tmpMat = m_parent ? m_parent->m_transform.getTransformation() *
		m_transform.getTransformation() : m_transform.getTransformation();
	for(auto& a : thing)
	{

		a = (tmpMat * a);


		//if(first)
		//{
		//	m_front = m_back = m_left = m_right = m_top = m_bottom = Coord3D(a.x, a.y, a.z);
		//	first = false;
		//}
		//else
		//{
		//	m_front = a.z > m_front.z ? Coord3D(a.x, a.y, a.z) : m_front;
		//	m_back = a.z < m_back.z ? Coord3D(a.x, a.y, a.z) : m_back;
		//	m_left = a.x < m_left.x ? Coord3D(a.x, a.y, a.z) : m_left;
		//	m_right = a.x > m_right.x ? Coord3D(a.x, a.y, a.z) : m_right;
		//	m_top = a.y > m_top.y ? Coord3D(a.x, a.y, a.z) : m_top;
		//	m_bottom = a.y < m_bottom.y ? Coord3D(a.x, a.y, a.z) : m_bottom;
		//}
	}
	m_right = *(Coord3D*)& thing[0];
	m_left = *(Coord3D*)& thing[1];
	m_top = *(Coord3D*)& thing[2];
	m_bottom = *(Coord3D*)& thing[3];
	m_front = *(Coord3D*)& thing[4];
	m_back = *(Coord3D*)& thing[5];

	//glm::vec4
	//	top = glm::vec4(*(glm::vec3*) & m_top, 1),
	//	bottom = glm::vec4(*(glm::vec3*) & m_bottom.x, 1),
	//	left = glm::vec4(*(glm::vec3*) & m_left, 1),
	//	right = glm::vec4(*(glm::vec3*) & m_right, 1),
	//	front = glm::vec4(*(glm::vec3*) & m_front, 1),
	//	back = glm::vec4(*(glm::vec3*) & m_back, 1);


	if(m_enableBB)
		boundingBoxInit();
}

Animation* Model::getAnimation(const char* tag)
{
	return m_animations[tag];
}

Animation* Model::getCurrentAnimation()
{
	return m_animations[m_animation];
}

void Model::setAnimation(const char* tag)
{
	m_animation = tag;
}

Mesh* Model::getMesh()
{
	return &m_mesh;
}

Shader* Model::getShader()
{
	return m_shader;
}

void Model::setToRender(bool render)
{
	m_render = render;
}

void Model::setTransparent(bool trans)
{
	m_transparent = trans;
}

bool Model::isTransparent()
{
	return m_transparent;
}

void Model::boundingBoxInit()
{
	if(!m_BBVaoID)
		glGenVertexArrays(1, &m_BBVaoID);
	if(!m_BBVboID)
		glGenBuffers(1, &m_BBVboID);

	float
		top = m_top.y,
		bottom = m_bottom.y,
		left = m_left.x,
		right = m_right.x,
		front = m_front.z,
		back = m_back.z;

	Vertex3D
		topLeftBack{{ left,top,back }},
		topRightBack{{ right,top,back }},
		topLeftFront{{ left, top,front }},
		topRightFront{{ right,top,front }},
		bottomLeftBack{{ left, bottom,back }},
		bottomRightBack{{ right,bottom,back }},
		bottomLeftFront{{ left, bottom,front }},
		bottomRightFront{{ right,bottom,front }};


	Vertex3D tmp[12 * 3]{
		//top
		topLeftBack,topRightBack,topRightFront,
		topLeftBack,topRightFront,topLeftFront,
		//bottom
		bottomLeftBack,bottomRightBack, bottomRightFront,
		bottomLeftBack,bottomRightFront,bottomLeftFront,
		//front
		topLeftFront,topRightFront,bottomRightFront,
		topLeftFront,bottomRightFront,bottomLeftFront,
		//back
		topLeftBack,  topRightBack,bottomRightBack,
		topLeftBack,bottomRightBack,bottomLeftBack,
		//left
		topLeftBack,topLeftFront,bottomLeftFront,
		topLeftBack,bottomLeftFront,bottomLeftBack,
		//right
		topRightBack,   topRightFront,bottomRightFront,
		topRightBack,bottomRightFront,bottomRightBack
	};

	memcpy_s(m_vertBBDat, sizeof(Vertex3D) * 12 * 3, tmp, sizeof(Vertex3D) * 12 * 3);

	glBindVertexArray(m_BBVaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_BBVboID);
	glBufferData(GL_ARRAY_BUFFER, 12 * 3 * sizeof(Vertex3D), m_vertBBDat, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//vertex     atributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

	//UV         atributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

	//normal     atributes
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}