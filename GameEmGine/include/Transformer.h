#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Quat.h"
#include "Utilities.h"
#include "Component.h"

class Transformer:public Component
{
public:
	
	Transformer();
	Transformer(Transformer&, COMP_TYPE type="TRANSFORMER");
	Transformer(COMP_TYPE type);
	~Transformer();

	void reset();
	void enableFPSMode(bool enable = true);

	/*SET ROTATION*/

	virtual void rotate(Coord3D<> angles);
	virtual void rotate(float x,float y ,float z);
	virtual void rotateBy(Coord3D<> angles);
	virtual void rotateBy(float x, float y, float z);

	/*SET POSITION*/

	virtual void translate(float x, float y, float z);
	virtual void translate(Coord3D<> pos);
	virtual void translateBy(float x, float y, float z);
	virtual void translateBy(Coord3D<> pos);


	/*SET SCALE*/

	virtual void scaleBy(float scale);
	virtual void scaleBy(float x, float y, float z);
	virtual void setScale(Coord3D<> scale);
	virtual void setScale(float scale);
	virtual void setScale(float x, float y, float z);

	/*GETTERS*/
	virtual Coord3D<> getPosition();
	virtual Coord3D<> getRotation();
	virtual Coord3D<> getScale();
	Coord3D<> getForward();
	Coord3D<> getUp();
	Coord3D<> getRight();

	virtual const glm::mat4& getLocalRotationMatrix();
	virtual const glm::mat4& getLocalScaleMatrix();
	virtual const glm::mat4& getLocalTranslationMatrix();
	 					   
	virtual const glm::mat4& getWorldRotationMatrix();
	virtual const glm::mat4& getWorldScaleMatrix();
	virtual const glm::mat4& getWorldTranslationMatrix();
	
	/*Gets a combination of the rotation, scale, and translation matricies*/

	virtual glm::mat4 getLocalTransformation();

	virtual glm::mat4 getWorldTransformation();

	virtual void resetUpdated();
	virtual bool isUpdated();
	virtual bool isScaleUpdated();
	virtual bool isRotationUpdated();
	virtual bool isTranslatinUpdated();

	virtual void addChild(Transformer* child);
	virtual void removeChild(Transformer* child);
	virtual void removeChild(unsigned index);
	virtual void setParent(Transformer* parent);
	virtual void removeParent(Transformer* parent);

	virtual Transformer* getChild(unsigned int index);
	virtual Transformer* getParent();
	virtual std::vector<Transformer*>& getChildren();

private:

	void calculateWorldRotationMatrix();
	void calculateWorldScaleMatrix();
	void calculateWorldTranslationMatrix();


	Coord3D<> m_forward = {0,0,1}, m_up = {0,1,0}, m_right = {1,0,0};
	Coord3D<> m_posDat, m_rotDat, m_scaleDat;
	std::vector<Transformer* >m_children;
	Transformer* m_parent;
	bool  m_updatedRot = true,
		m_updatedTrans = true,
		m_updatedScale = true,
		//first person movement
		m_fps=false,
		m_rotateBy=false;

protected:
	glm::mat4
		m_localTranslate,
		m_localRotate,
		m_localScale,

		m_worldTranslate,
		m_worldRotate,
		m_worldScale;

};

