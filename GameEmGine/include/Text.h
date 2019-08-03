#pragma once

#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Transformer.h"
#include "ResourceManager.h"


class Camera;

class Text:public Transformer,public Character
{
public:
	Text();
	Text(const char* font);

	void setText(std::string text);

	void textSize(short s);

	void textColour(ColourRGBA);

	uint size();

	void renderInit();

	void render(Shader& s, Camera* cam);

	bool isTransparent();
	//static Character loadCharacter(const char c, const char* font);

private:

	ColourRGBA m_colour;
	std::string m_font, m_text;
	uint m_length;
	GLuint m_vaoID, m_vboID;
};

