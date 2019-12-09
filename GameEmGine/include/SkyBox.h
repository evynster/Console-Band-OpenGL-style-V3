#pragma once
#include <string>
#include "mesh.h"
#include "ResourceManager.h"
#include "Camera.h"
class SkyBox
{
public:
	SkyBox(std::string file = "");
	~SkyBox() = default;

	void render();

	void setCubeMap(cstring path);
	void setCamera(Camera* cam);

private:
	void init();

	Texture3D* m_sky;
	static primitiveCube m_box;

	Camera* m_camera;
	GLuint m_vaoID, m_vboID, m_eleID;

};

