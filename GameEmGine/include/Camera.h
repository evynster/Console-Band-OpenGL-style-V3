#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transformer.h"
#include "Utilities.h"
#include "Quat.h"
#include "Shader.h"
#include "Model.h"

class Model;

enum CAMERA_TYPE
{
	ORTHOGRAPHIC,
	FRUSTUM
};

struct projectionPeramiters
{
	virtual ~projectionPeramiters() = 0;
	float front, back;
};

struct OrthoPeramiters:public projectionPeramiters
{
	float  left, right, bottom, top;
};

struct FrustumPeramiters:public projectionPeramiters
{
	float angle,aspect, depth;
};
class Camera
{
public:
	Camera(Size3D = {1,1,1}, CAMERA_TYPE = FRUSTUM);
	~Camera();

	void init(Size3D = {}, CAMERA_TYPE = FRUSTUM);
	void setType(CAMERA_TYPE type, projectionPeramiters* peram = nullptr);
	CAMERA_TYPE getType();
	void enableFPS(bool enable = true);
	bool update();

	void translate(Coord3D<>);
	void movePositionBy(Coord3D<> position);
	void setScale(const float);
	void rotate(float angle, Coord3D<> direction);

	void rotateBy(float angle, Coord3D<> direction);

	void render(Shader* shader, std::vector<Model*>& models, bool transparent = false);


	Coord3D<> getPosition();
	float& getScale();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getCameraMatrix();

	glm::mat4 getObjectMatrix();
	Transformer& getTransformer();

protected:
	bool m_cameraUpdate, m_isTranslate, m_isTranslateBy;
	float m_scale;
	Size3D* m_size = new Size3D;
	Coord3D<>* m_position, * m_positionBy;
	CAMERA_TYPE m_type = FRUSTUM;

	Transformer m_transform;
	glm::mat4 m_cameraMat;
	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_objMat;
};

