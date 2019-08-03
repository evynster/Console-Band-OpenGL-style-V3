#pragma once
//#define clamp(val, min, max) (val > max ? max : val < min ? min : val)
#include <GL/glew.h>
#include <glm/common.hpp>
#include <string>

#define reclass(a_class,a_val) (*(a_class*)&(a_val))
#define uint unsigned int
template<class T = float>
struct Coord2D
{
	T x = 0, y = 0;

	glm::vec2 toVec2()
	{
		return glm::vec2(x, y);
	}
	Coord2D() = default;

	Coord2D(T a_x, T a_y)
	{
		x = a_x, y = a_y;
	}
	T distance()
	{
		return sqrtf(x * x + y * y);
	}

	static T distance(Coord2D v1, Coord2D v2)
	{
		Coord2D v3 = v2 - v1;
		return sqrtf(v3.x * v3.x + v3.y * v3.y);
	}

	Coord2D normal()
	{
		return *this / distance();
	}

	T& operator[](int m_index)
	{
		T* error = nullptr;
		switch(m_index)
		{
		case 0:
			return static_cast<T&>(x);
		case 1:
			return static_cast<T&>(y);
		}
		return *error;
	}

	Coord2D operator+(Coord2D coord)
	{
		return {x + coord.x, y + coord.y};
	}
	Coord2D operator-(Coord2D coord)
	{
		return {x - coord.x, y - coord.y};
	}
	Coord2D operator/(Coord2D coord)
	{
		return {x / coord.x,y / coord.y};
	}
	Coord2D operator/(T coord)
	{
		return {x / coord,y / coord};
	}
	void operator-=(Coord2D coord)
	{
		x -= coord.x;
		y -= coord.y;
	}
	void operator+=(Coord2D coord)
	{
		x += coord.x;
		y += coord.y;
	}
	void operator/=(Coord2D coord)
	{
		x /= coord.x, y /= coord.y;
	}
	void operator/=(T coord)
	{
		x /= coord, y /= coord;
	}
};

template<class T = float>
struct Coord3D
{
	T x = 0.0f, y = 0.0f, z = 0.0f;

	Coord3D() = default;

	Coord3D(Coord2D<T> coord)
	{
		x = coord.x;
		y = coord.y;
	}

	Coord3D(T scale)
	{
		this->x = scale;
		this->y = scale;
		this->z = scale;
	}

	Coord3D(T m_x, T m_y, T m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	Coord3D(T m_x, T m_y)
	{
		this->x = m_x;
		this->y = m_y;
	}

	glm::vec3 toVec3()
	{
		return glm::vec3(x, y, z);
	}

	static T distance(Coord3D v1, Coord3D v2)
	{
		v1 -= v2;
		return v1.distance();
	}

	T distance()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	T distanceSquare()
	{
		return (x * x + y * y + z * z);
	}

	static T dotProduct(Coord3D a, Coord3D b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Coord3D crossProduct(Coord3D a, Coord3D b)
	{
		return
		{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}
	friend static Coord3D abs(Coord3D val)
	{
		return {sqrtf(val.x * val.x),sqrtf(val.y * val.y),sqrtf(val.z * val.z)};
	}


	Coord3D normal()
	{
		return *this / distance();
	}


	void set(Coord2D<T> coord)
	{
		x = coord.x;
		y = coord.y;
	}

	void set(T m_x, T m_y, T m_z)
	{
		this->x = m_x;
		this->y = m_y;
		this->z = m_z;
	}

	void set(T m_x, T m_y)
	{
		this->x = m_x;
		this->y = m_y;
	}

	void normalize()
	{
		T norm = sqrtf(x * x + y * y + z * z);
		x /= norm;
		y /= norm;
		z /= norm;
	}

	T& operator[] (int m_index)
	{
		T* error = nullptr;
		switch(m_index)
		{
		case 0:
			return const_cast<T&>(x);
		case 1:
			return const_cast<T&>(y);
		case 2:
			return const_cast<T&>(z);
		}
		return *error;
	}

	Coord3D operator+(Coord3D coord)
	{
		return {x + coord.x, y + coord.y, z + coord.z};
	}

	Coord3D operator-(Coord3D coord)
	{
		return {x - coord.x, y - coord.y, z - coord.z};
	}

	friend Coord3D operator*(T scaler, Coord3D coord)
	{
		return {scaler * coord.x, scaler * coord.y, scaler * coord.z};
	}

	Coord3D operator*(Coord3D coord)
	{
		return {x * coord.x, y * coord.y, z * coord.z};
	}

	Coord3D operator*(T coord)
	{
		return {x * coord, y * coord, z * coord};
	}

	Coord3D operator/(Coord3D coord)
	{
		return {x / coord.x,y / coord.y,z / coord.z};
	}

	Coord3D operator/(T coord)
	{
		return {x / coord,y / coord,z / coord};
	}

	Coord3D& operator-() {
		static Coord3D tmp;
		tmp = *this * -1;
		return tmp;
	}

	void operator-=(Coord3D coord)
	{
		x -= coord.x;
		y -= coord.y;
		z -= coord.z;
	}

	void operator+=(Coord3D coord)
	{
		x += coord.x;
		y += coord.y;
		z += coord.z;
	}

	void operator*=(Coord3D coord)
	{
		x *= coord.x;
		y *= coord.y;
		z *= coord.z;
	}

	void operator*=(T coord)
	{
		x *= coord;
		y *= coord;
		z *= coord;
	}

	bool operator==(Coord3D coord)const
	{
		return
			x == coord.x &&
			y == coord.y &&
			z == coord.z;
	}

	bool operator!=(Coord3D coord)const
	{
		return !(*this == coord);
	}

	bool operator>(Coord3D coord)
	{
		return this->distanceSquare() > coord.distanceSquare();
	}

	bool operator<=(Coord3D coord)
	{
		return !(*this > coord);
	}
	bool operator<(Coord3D coord)
	{
		return this->distanceSquare() < coord.distanceSquare();
	}
	bool operator>=(Coord3D coord)
	{
		return !(*this < coord);
	}
};

struct Size2D
{
	float width = 0, height = 0;
	float& operator[](int m_index)
	{
		float* error = nullptr;
		switch(m_index)
		{
		case 0:
			return static_cast<float&>(width);
		case 1:
			return static_cast<float&>(height);
		}
		return *error;
	}
};

struct Size3D
{
	float width = 0, height = 0, depth = 0;

	Size3D()
	{}

	Size3D(Size2D size)
	{
		width = size.width;
		height = size.height;
	}

	Size3D(float w, float h, float d)
	{
		this->width = w;
		this->height = h;
		this->depth = d;
	}

	Size3D(float w, float h)
	{
		this->width = w;
		this->height = h;
	}

	void set(Size2D size)
	{
		width = size.width;
		height = size.height;
	}

	void set(float w, float h, float d)
	{
		this->width = w;
		this->height = h;
		this->depth = d;
	}

	void set(float w, float h)
	{
		this->width = w;
		this->height = h;
	}

	float& operator[] (int m_index)
	{
		float* error = nullptr;
		switch(m_index)
		{
		case 0:
			return const_cast<float&>(width);
		case 1:
			return const_cast<float&>(height);
		case 2:
			return const_cast<float&>(depth);
		}
		return *error;
	}
};

struct vboInfo3D
{
	Coord3D<> pos;
	Size3D size;
};

struct ColourRGBA
{
	GLubyte r, g, b, a;

	ColourRGBA():r((GLubyte)255), g((GLubyte)255), b((GLubyte)255), a((GLubyte)255)
	{}

	ColourRGBA(GLubyte a_r, GLubyte a_g, GLubyte a_b, GLubyte a_a = (GLubyte)255):r(a_r), g(a_g), b(a_b), a(a_a)
	{}

	void set(ColourRGBA rgba)
	{
		set(
			rgba.r,
			rgba.g,
			rgba.b,
			rgba.a);
	}

	void set(GLubyte a_r, GLubyte a_g, GLubyte a_b)
	{
		this[0][0] = a_r;
		this[0][1] = a_g;
		this[0][2] = a_b;
	}

	void set(GLubyte a_r, GLubyte a_g, GLubyte a_b, GLubyte a_a)
	{
		this[0][0] = a_r;
		this[0][1] = a_g;
		this[0][2] = a_b;
		this[0][3] = a_a;
	}
	
	void set(float a_r, float a_g, float a_b)
	{
		this[0][0] = (GLubyte)a_r * 255;
		this[0][1] = (GLubyte)a_g * 255;
		this[0][2] = (GLubyte)a_b * 255;
	}

	void set(float a_r, float a_g, float a_b, float a_a)
	{
		this[0][0] = (GLubyte)a_r * 255;
		this[0][1] = (GLubyte)a_g * 255;
		this[0][2] = (GLubyte)a_b * 255;
		this[0][3] = (GLubyte)a_a * 255;
	}

	ColourRGBA operator+(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(r + rgba[0]),
			GLubyte(g + rgba[1]),
			GLubyte(b + rgba[2]),
			GLubyte(a + rgba[3])};
	}

	ColourRGBA operator*(float rgba)
	{
		return ColourRGBA{
			GLubyte(r * rgba),
			GLubyte(g * rgba),
			GLubyte(b * rgba),
			GLubyte(a * rgba)};
	}

	friend ColourRGBA operator*(float rgba, ColourRGBA colour)
	{
		return ColourRGBA{
			GLubyte(colour[0] * rgba),
			GLubyte(colour[1] * rgba),
			GLubyte(colour[2] * rgba),
			GLubyte(colour[3] * rgba)};
	}
	ColourRGBA operator*(ColourRGBA rgba)
	{
		return ColourRGBA{
			GLubyte(r * (float)rgba.r / 255),
			GLubyte(g * (float)rgba.g / 255),
			GLubyte(b * (float)rgba.b / 255),
			GLubyte(a * (float)rgba.a / 255)};
	}

	void operator*=(ColourRGBA rgba)
	{
		*this = *this * rgba;
	}

	GLubyte& operator[](int m_index)
	{
		GLubyte* error = nullptr;
		switch(m_index)
		{
		case 0:
			return static_cast<GLubyte&>(r);
		case 1:
			return static_cast<GLubyte&>(g);
		case 2:
			return static_cast<GLubyte&>(b);
		case 3:
			return static_cast<GLubyte&>(a);
		}
		return *error;
	}
};

struct UV
{
	float uv_u = 0, uv_v = 0, uv_w = 0;
	void set(float u, float v, float w)
	{
		this->uv_u = u;
		this->uv_v = v;
		this->uv_w = w;
	}

	void set(float u, float v)
	{
		this->uv_u = u;
		this->uv_v = v;
	}

	float& operator[](int m_index)
	{
		switch(m_index)
		{
		case 0:
			return static_cast<float&>(uv_u);
		case 1:
			return static_cast<float&>(uv_v);
		case 2:
			return static_cast<float&>(uv_w);
		}
		float* error = nullptr;
		return *error;
	}
};

struct VboInfo2D
{
	VboInfo2D(Coord2D<> c = {0,0}, Size2D s = {0,0})
	{
		position = c;
		size = s;
	}

	Coord2D<> position;
	Size2D size;
protected:
	float _angle;
};

struct Vertex2D
{
	Coord2D<> coord;
	ColourRGBA	colour;
	UV uv;

	void setCoord2D(float x, float y)
	{
		coord.x = x;
		coord.y = y;
	}
	//uses the 0-255 representation instead of 0-1
	void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
	{
		colour.r = r;
		colour.g = g;
		colour.b = b;
		colour.a = a;
	}
	//sets uv
	void setUV(float u, float v)
	{
		uv.uv_u = u;
		uv.uv_v = v;
	}

	void print()
	{
		printf("Coord2D: (%f, %f)\n", coord.x, coord.y);
		printf("Colour : (%d, %d, %d, %d)\n", colour.r, colour.g, colour.b, colour.a);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct Vertex3D
{
	Coord3D<> coord, norm;
	ColourRGBA	colour;
	UV uv;

	void setCoord(float x, float y, float z)
	{
		coord.x = x;
		coord.y = y;
		coord.z = z;
	}
	//uses the 0-255 representation instead of 0-1
	void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
	{
		colour.r = r;
		colour.g = g;
		colour.b = b;
		colour.a = a;
	}
	//sets uv
	void setUV(float u, float v)
	{
		uv.uv_u = u;
		uv.uv_v = v;
	}

	void setNorm(float x, float y, float z)
	{
		norm.x = x;
		norm.y = y;
		norm.z = z;
	}

	void print()
	{
		printf("Coord3D: (%f, %f, %f)\n", coord.x, coord.y, coord.z);
		printf("Colour : (%d, %d, %d, %d)\n", colour.r, colour.g, colour.b, colour.a);
		printf("UV     : (%f, %f)\n\n", uv.uv_u, uv.uv_v);
	}
};

struct WindowInfo
{
	std::string* title = new std::string;
	Size3D* size = new Size3D;
	Coord2D<>* position = new Coord2D<>;
	int monitor;
	void print()
	{
		printf("Title    : %s\n\n", title->c_str());
		printf("Position : (%f, %f)\n", position->x, position->y);
		printf("Size     : (%.0f, %.0f, %.0f)\n", size->width, size->height, size->depth);
		printf("Monitor  : %d\n\n", monitor);
	}
};