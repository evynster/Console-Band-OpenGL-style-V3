#pragma once
#include <GL\glew.h>
#include "Utilities.h"

enum class TEXTURE_TYPE2D :unsigned
{
	NONE,
	DIFFUSE,
	SPECULAR
};

enum class TEXTURE_TYPE3D :unsigned
{
	NONE,
	LUT,
	CUBE
};

struct Texture2D
{
	GLuint id;
	ColourRGBA colour;
	int width, height;
	TEXTURE_TYPE2D type = TEXTURE_TYPE2D::NONE;
	std::string name;


	void deleteTexture()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	void bindTexture(unsigned slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	
	void bindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	static void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	static void unbindTexture(unsigned slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	bool operator==(Texture2D arg)
	{
		return id == arg.id;
	}
};

struct Texture3D
{
	GLuint id;
	int width, height, depth;
	int lutSize;
	TEXTURE_TYPE3D type;
	Texture3D(TEXTURE_TYPE3D aType = TEXTURE_TYPE3D::NONE) :type(aType) { };

	void deleteTexture()
	{
		if (id)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}
	}

	void bindTexture()
	{
		switch (type)
		{
		case TEXTURE_TYPE3D::LUT:
			glBindTexture(GL_TEXTURE_3D, id);
			break;
		case TEXTURE_TYPE3D::CUBE:
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			break;
		default:
			puts("undefined type\n");
		}
	}



	void unbindTexture()
	{
		switch (type)
		{
		case TEXTURE_TYPE3D::LUT:
			glBindTexture(GL_TEXTURE_3D, 0);
			break;
		case TEXTURE_TYPE3D::CUBE:
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			break;
		default:
			puts("undefined type\n");
		}
	}

	bool operator==(Texture3D arg)
	{
		return id == arg.id;
	}
private:
};


