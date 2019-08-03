#include "Text.h"



Text::Text():Transformer(), m_vaoID(0), m_vboID(0)
{

	m_type = TEXT;
	m_font = "fonts/arial.ttf";

	//printf("%s\n", m_face->style_name);
}

Text::Text(const char* font):Transformer(), m_vaoID(0), m_vboID(0)
{

	m_type = TEXT;
	m_font = font;

	//if(FT_Init_FreeType(&m_ft))
	//	printf("ERROR::FREETYPE: Could not init FreeType Library\n");
	//
	//if(FT_New_Face(m_ft, font, 0, &m_face))
	//	printf("ERROR::FREETYPE: Failed to load font \"%s\"\n", font);
	//m_face->style_name;
}

void Text::setText(std::string text)
{
	m_text = text;
}

void Text::textSize(short s)
{
	setScale(s);
}

void Text::textColour(ColourRGBA colour)
{
	m_colour = colour;
}

uint Text::size()
{
	return m_length;
}

void Text::renderInit()
{
	glGenVertexArrays(1, &m_vaoID);
	glGenBuffers(1, &m_vboID);
	glBindVertexArray(m_vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Text::render(Shader& s, Camera* cam)
{
	if(!m_vaoID || !m_vboID)
		renderInit();

	// Activate corresponding render state	
	s.enable();

	glUniformMatrix4fv(s.getUniformLocation("uModel"), 1, GL_FALSE, &(getTransformation()[0][0]));
	glUniformMatrix4fv(s.getUniformLocation("uView"), 1, GL_FALSE, &((cam->getObjectMatrix() * cam->getViewMatrix())[0][0]));
	glUniformMatrix4fv(s.getUniformLocation("uProj"), 1, GL_FALSE, &(cam->getProjectionMatrix()[0][0]));

	float colour[4]{(float)m_colour.r / 255,(float)m_colour.g / 255,(float)m_colour.b / 255,(float)m_colour.a / 255};

	s.sendUniform("textColor", colour[0], colour[1], colour[2], colour[3]);
	//glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vaoID);

	float x = getPosition().x,
		y = getPosition().y,
		aspect;

	int winW, winH;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &winW, &winH);
	aspect = (float)winH;

	// Iterate through all characters
	for(auto& c : m_text)
	{
		Character ch = ResourceManager::getCharacter(c, m_font.c_str());

		GLfloat xpos = (x + ch.bearing.x * getScale().x)/aspect;
		GLfloat ypos = (y - (ch.size.y - ch.bearing.y) * getScale().x) / aspect;

		GLfloat w = ch.size.x * getScale().x / aspect;
		GLfloat h = ch.size.y * getScale().x / aspect;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * getScale().x; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Text::isTransparent()
{
	return false;
}


