#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include "Transformer.h"
#include "Utilities.h"
#include "Quat.h"
#include "Shader.h"
#include "Model.h"
#include "Text.h"

class Model;

enum CAMERA_TYPE
{
	ORTHOGRAPHIC,
	FRUSTUM
};

struct ProjectionPeramiters
{
	ProjectionPeramiters(float znear, float zfar):zNear(znear), zFar(zfar) {}
	~ProjectionPeramiters()=default;

	void setNear(float znear) { zNear = znear; }
	void setFar(float zfar) { zFar = zfar; }
	void setZAxis(float znear, float zfar) { zNear = znear; zFar = zfar; }

	float zNear, zFar;
};

struct OrthoPeramiters:public ProjectionPeramiters
{
	OrthoPeramiters(float  a_left, float a_right, float a_bottom, float a_top, float a_near, float a_far):
		ProjectionPeramiters(a_near, a_far),
		left(a_left),right(a_right),bottom(a_bottom),top(a_top){}

	void setLeft(float a_left) { left = a_left; }
	void setRight(float a_right) { right = a_right; }
	void setBottom(float a_bottom) { bottom = a_bottom; }
	void setTop(float a_top) { top = a_top; }

	void setXAxis(float a_left, float a_right) { left = a_left; right = a_right; }
	void setYAxis(float a_top, float a_bottom) { top = a_top; bottom = a_bottom; }
	float  left, right, bottom, top;
};

struct FrustumPeramiters:public ProjectionPeramiters
{
	FrustumPeramiters(float a_angle, float a_aspect, float a_near, float a_far):
		ProjectionPeramiters(a_near, a_far),
		angle(a_angle), aspect(a_aspect) {}

	void setAngle(float a_angle) { angle = a_angle; }
	void setAspect(float a_aspect) { aspect = a_aspect; }
	float angle, aspect;
};
class Camera
{
public:
	Camera(Size3D = {1,1,1}, CAMERA_TYPE = FRUSTUM, ProjectionPeramiters* peram = nullptr);
	~Camera();

	void init(Size3D = {}, CAMERA_TYPE = FRUSTUM, ProjectionPeramiters* peram = nullptr);
	void setType(CAMERA_TYPE type, ProjectionPeramiters* peram = nullptr);
	CAMERA_TYPE getType();
	void enableFPS(bool enable = true);
	bool update();

	void translate(Coord3D<>);
	void movePositionBy(Coord3D<> position);
	void setScale(const float);
	void rotate(float angle, Coord3D<> direction);

	void rotateBy(float angle, Coord3D<> direction);

	void render(Shader* shader, std::map<void*, Model*>& models, bool transparent = false);


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

