#include "SkyBox.h"

primitiveCube SkyBox::m_box(2, 2, 2, {}, {}, true);

SkyBox::SkyBox(std::string file):m_sky(&ResourceManager::getTextureCubeMap(file.c_str())) { init(); }

void SkyBox::init()
{
	glGenVertexArrays(1, &m_vaoID);
	glGenBuffers(1, &m_vboID);
	glGenBuffers(1, &m_eleID);

	glBindVertexArray(m_vaoID);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, m_box.getData().size() * sizeof(Vertex3D), m_box.getData().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eleID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_box.getIndices().size() * sizeof(unsigned), m_box.getIndices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SkyBox::render()
{
	//if(!m_sky) return;
	glDepthMask(GL_FALSE);

	static Camera cam({2, 2, 2},FRUSTUM);
	Shader* shader = ResourceManager::getShader("Shaders/SkyBox.vtsh", "Shaders/SkyBox.fmsh");
	//glDisable(GL_DEPTH_TEST);
	shader->enable();

	shader->sendUniform("uView",glm::inverse(m_camera->getLocalRotationMatrix()));
	shader->sendUniform("uProj", m_camera->getProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	m_sky->bindTexture();
	shader->sendUniform("sky", 0);


	glBindVertexArray(m_vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eleID);

	glDrawElements(
		GL_TRIANGLES,//draw pattern
		m_box.getIndices().size(),//amout of indicies
		GL_UNSIGNED_INT,//data type
		(void*)0 //indice offset 
	);
	m_sky->unbindTexture();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader->disable();
	glDepthMask(GL_TRUE);

}

void SkyBox::setCubeMap(cstring path)
{
	m_sky = &ResourceManager::getTextureCubeMap(path);
}

void SkyBox::setCamera(Camera* cam)
{
	m_camera = cam;
}
