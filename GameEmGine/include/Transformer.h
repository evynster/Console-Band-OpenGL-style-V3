#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Quat.h"
#include "Utilities.h"

enum CLASS_TYPE
{
	TRANSFORMER,
	MODEL,
	TEXT
};

class Transformer
{
public:

	Transformer();
	~Transformer();

	void reset();
	void enableFPSMode(bool enable = true);
	/*SET ROTATION*/

	void rotate(Coord3D<> direction);
	void rotateBy(Coord3D<> direction);

	/*SET POSITION*/

	void translate(float x, float y, float z);
	void translate(Coord3D<> pos);
	void translateBy(float x, float y, float z);
	void translateBy(Coord3D<> pos);


	/*SET SCALE*/

	virtual void scaleBy(float scale);
	virtual void scaleBy(float x, float y, float z);
	virtual void setScale(Coord3D<> scale);
	virtual void setScale(float scale);
	virtual void setScale(float x, float y, float z);

	/*GETERS*/
	Coord3D<> getPosition();
	Coord3D<> getRotation();
	Coord3D<> getScale();
	glm::mat4 getRotationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getTranslationMatrix();

	/*Gets a combination of the rotation, scale, and translation matricies*/
	glm::mat4 getTransformation();

	void resetUpdated();

	bool isUpdated();
	bool isScaleUpdated();
	bool isRotationUpdated();
	bool isTranslatinUpdated();

	virtual void addChild(Transformer* child);
	void removeChild(Transformer* child);
	virtual Transformer* getChild(unsigned int index);
	virtual std::vector<Transformer*>& getChildren();

	CLASS_TYPE getType();
private:

	Coord3D<> m_posDat, m_rotDat, m_scaleDat;
	Coord3D<> m_forward = {0,0,1}, m_up = {0,1,0}, m_right = {1,0,0};
	glm::mat4
		m_translate,
		m_rotate,
		m_scale;

	std::vector<Transformer* >m_children;
	Transformer* m_parent;
	bool  m_updatedRot = true,
		m_updatedTrans = true,
		m_updatedScale = true,
		m_fps;

protected:
	CLASS_TYPE m_type;
};

