#pragma once

#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Transformer.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"
//#include "Utilities.h"



class Camera;

class Text:public Transformer, public Character
{
public:
	Text();
	Text(const char* font);

	~Text();

	void setText(std::string text);

	void textSize(short s);

	void setColour(float r, float g, float b);

	void setColour(ColourRGBA colour);

	unsigned int size();

	void render(Shader& s, Camera* cam, bool texture = false);

	void toFramebufferTexture();

	FrameBuffer* getFramebuffer();

	bool isTransparent();
	//static Character loadCharacter(const char c, const char* font);

private:


	/*SET SCALE*/

	void scaleBy(float scale) { Transformer::scaleBy(scale); }
	void scaleBy(float x, float y, float z) { Transformer::scaleBy(x,y,z); }
	void setScale(Coord3D<> scale) { Transformer::setScale(scale); }
	void setScale(float scale) { Transformer::setScale(scale); }
	void setScale(float x, float y, float z) { Transformer::setScale(x,y,z); }

	void renderInit();

	FrameBuffer* m_texture;
	ColourRGBA m_colour;
	std::string m_font, m_text;
	unsigned int m_length;
	GLuint m_vaoID, m_vboID;
	
	static float m_initY;
};

