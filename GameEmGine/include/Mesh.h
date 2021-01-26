#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "Transformer.h"
#include "Shader.h"
#include "Utilities.h"
#include "ResourceManager.h"

#define CHAR_BUFF_SIZE 1000

enum PRIMITIVE_TYPE
{
	NONE,
	PLANE,
	CUBE,
	SPHEAR
};

struct primitiveMesh
{
	primitiveMesh() {}
	primitiveMesh(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}):
		m_dim(dim), m_offset(offset), m_rotation(rot)
	{}
	primitiveMesh(float width, float height, float depth, Coord3D<> offset = {}, Coord3D<> rot = {}):
		m_dim({width,height,depth}), m_offset(offset), m_rotation(rot)
	{}

	virtual ~primitiveMesh() {}

	virtual void setDimentions(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}) { m_dim = dim; m_offset = offset; m_rotation = rot; }
	virtual void setDimentions(float w, float h, float d = 0, Coord3D<> offset = {}, Coord3D<> rot = {})
	{
		m_dim = {w,h,d}; m_offset = offset; m_rotation = rot;
	}
	virtual void createMesh() = 0;

	Coord3D<> getDimentions() { return m_dim; }

	PRIMITIVE_TYPE& getType() { return type; }

	std::vector<Vertex3D>& getData() { return m_unpackedData; };
	std::vector<unsigned>& getIndices() { return m_indices; };


	Coord3D<> m_top, m_bottom, m_left, m_right, m_front, m_back;
protected:
	Coord3D<> m_dim, m_offset, m_rotation;

	PRIMITIVE_TYPE type = NONE;
	std::vector<Vertex3D> m_unpackedData;
	std::vector<unsigned> m_indices;

	//adds indices to the existing structure
	template<class _iter>
	void indiceAdder(_iter begin, _iter end, std::vector<Vertex3D> verts)
	{
		for(auto& i = begin; i < end; ++i)
		{
			std::vector<Vertex3D>::iterator tmp;
			if((tmp = std::find(m_unpackedData.begin(), m_unpackedData.end(), verts[*i])) != m_unpackedData.end())
			{
				m_indices.push_back(unsigned(tmp - m_unpackedData.begin()));
			}
			else
			{
				m_unpackedData.push_back(verts[*i]);
				m_indices.push_back(m_unpackedData.size() - 1);
			}
		}
	}

	//clears all mesh data before adding indices
	void indiceCreator(std::vector<Vertex3D> verts)
	{
		m_unpackedData.clear();
		m_indices.clear();

		for(auto i = verts.begin(); i < verts.end(); ++i)
		{
			std::vector<Vertex3D>::iterator tmp;
			if((tmp = std::find(m_unpackedData.begin(), m_unpackedData.end(), *i)) != m_unpackedData.end())
			{
				m_indices.push_back(unsigned(tmp - m_unpackedData.begin()));
			}
			else
			{
				m_unpackedData.push_back(*i);
				m_indices.push_back(m_unpackedData.size() - 1);
			}
		}
	}
};

struct PrimitivePlane:public primitiveMesh
{
	PrimitivePlane():primitiveMesh() { type = PLANE; }

	PrimitivePlane(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}):primitiveMesh(dim, offset, rot) { type = PLANE; createMesh(); }

	PrimitivePlane(float w, float h, float d = 0, Coord3D<> offset = {}, Coord3D<> rot = {})
		:primitiveMesh(w, h, d, offset, rot)
	{
		type = PLANE; createMesh();
	}

	~PrimitivePlane() {};


	void createMesh()
	{
		float
			halfW = m_dim.width * .5f,
			halfH = m_dim.height * .5f,
			halfD = m_dim.depth * .5f;

		//coords
		std::vector<Vertex3D> tmp;
		glm::vec4 holder;
		//bottom left tri
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW,-halfH, halfD}).toVec3(), 1)
			)});//bottom left
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW,-halfH,-halfD}).toVec3(), 1)
			)});//bottom right*
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW, halfH,-halfD}).toVec3(), 1)
			)});//top left*

				  //top right tri
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW, halfH,-halfD}).toVec3(), 1)
			)});//top right
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW, halfH, halfD}).toVec3(), 1)
			)});//top left*
		tmp.push_back({m_offset + reclass(Coord3D<>,holder =
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW,-halfH, halfD}).toVec3(), 1)
			)});//bottom right*


		m_top = m_bottom = m_left = m_right = m_front = m_back = tmp[0].coord;

		for(unsigned int a = 1; a < tmp.size(); a++)
		{
			m_top = tmp[a].coord.y > m_top.y ? tmp[a].coord : m_top;
			m_bottom = tmp[a].coord.y < m_bottom.y ? tmp[a].coord : m_bottom;
			m_left = tmp[a].coord.x < m_left.x ? tmp[a].coord : m_left;
			m_right = tmp[a].coord.x > m_right.x ? tmp[a].coord : m_right;
			m_front = tmp[a].coord.z < m_front.z ? tmp[a].coord : m_front;
			m_back = tmp[a].coord.z > m_back.z ? tmp[a].coord : m_back;
		}


		//normal
		Coord3D<> norm = Coord3D<>::crossProduct(tmp[0].coord - tmp[1].coord, tmp[0].coord - tmp[2].coord).normal();

		tmp[0].norm = norm;
		tmp[1].norm = norm;
		tmp[2].norm = norm;

		tmp[3].norm = norm;
		tmp[4].norm = norm;
		tmp[5].norm = norm;

		Coord3D x, y;
		if(abs(norm) == Coord3D<>{0, 0, 1})
		{
			x = {1,0,0};
			y = {0,1,0};
		}
		else if(abs(norm) == Coord3D<>{0, 1, 0})
		{
			x = {1,0,0};
			y = {0,0,1};
		}
		else if(abs(norm) == Coord3D<>{1, 0, 0})
		{
			x = {0,0,1};
			y = {0,1,0};
		}

		//uv
		Coord3D<> val;

		val = ((tmp[0].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[0].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//bottom left 
		val = ((tmp[1].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[1].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//bottom right*
		val = ((tmp[2].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[2].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//top left*

		val = ((tmp[3].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[3].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//top right
		val = ((tmp[4].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[4].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//top left*
		val = ((tmp[5].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[5].uv = {float((val * x).distance() > 0),float((val * y).distance() > 0)};//bottom right*


		indiceCreator(tmp);
	}
};

struct primitiveCube: public primitiveMesh
{
public:
	primitiveCube():primitiveMesh() { type = CUBE; }

	primitiveCube(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}, bool invert = false):primitiveMesh(dim, offset, rot)
	{
		type = CUBE; m_invert = invert; createMesh();
	}

	primitiveCube(float w, float h, float d = 0, Coord3D<> offset = {}, Coord3D<> rot = {}, bool invert = false)
		:primitiveMesh(w, h, d, offset, rot)
	{
		type = CUBE; m_invert = invert; createMesh();
	}

	~primitiveCube() {}

	void createMesh()
	{
		static PrimitivePlane left, right, top, bottom, front, back;
		m_indices.clear();
		m_unpackedData.clear();

		float
			halfW = m_dim.width * .5f,
			halfH = m_dim.height * .5f,
			halfD = m_dim.depth * .5f;

		right.setDimentions({0, m_dim.height, m_dim.depth}, m_offset + Coord3D<>{halfW, 0, 0}), left.setDimentions(0, m_dim.height, m_dim.depth, m_offset + Coord3D<>{-halfW, 0, 0});
		top.setDimentions(m_dim.width, 0, m_dim.depth, m_offset + Coord3D<>{0, halfH, 0}), bottom.setDimentions(m_dim.width, 0, m_dim.depth, m_offset + Coord3D<>{0, -halfH, 0});
		front.setDimentions({m_dim.width, m_dim.height, 0}, m_offset + Coord3D<> {0, 0, -halfD}), back.setDimentions({m_dim.width, m_dim.height, 0}, m_offset + Coord3D<>{0, 0, halfD});

		front.createMesh(), back.createMesh(), left.createMesh(), right.createMesh(), top.createMesh(), bottom.createMesh();

		for(auto& a : left.getData())
			a.norm *= -1;
		for(auto& a : bottom.getData())
			a.norm *= -1;
		for(auto& a : front.getData())
			a.norm *= -1;

		indiceAdder(front.getIndices().rbegin(), front.getIndices().rend(), front.getData());
		indiceAdder(back.getIndices().begin(), back.getIndices().end(), back.getData());

		indiceAdder(left.getIndices().rbegin(), left.getIndices().rend(), left.getData());
		indiceAdder(right.getIndices().begin(), right.getIndices().end(), right.getData());

		indiceAdder(bottom.getIndices().rbegin(), bottom.getIndices().rend(), bottom.getData());
		indiceAdder(top.getIndices().begin(), top.getIndices().end(), top.getData());

		for(auto& a : m_unpackedData)
		{
			glm::mat4 rot = Quat::quatRotationMat(m_rotation.x, 1, 0, 0) * Quat::quatRotationMat(m_rotation.y, 0, 1, 0) * Quat::quatRotationMat(m_rotation.z, 0, 0, 1);
			glm::vec4 holder;
			a.coord = reclass(Coord3D<>, holder = rot * glm::vec4(a.coord.toVec3(), 1));
			a.norm = reclass(Coord3D<>, holder = rot * glm::vec4(a.norm.toVec3(), 1));
		}

		if(m_invert)
			for(unsigned a = 0; a <unsigned(m_indices.size() * .5f); ++a)
				std::swap(m_indices[a], m_indices[m_indices.size() - 1 - a]);


		m_top = m_bottom = m_left = m_right = m_front = m_back = m_unpackedData[0].coord;

		for(unsigned int a = 1; a < m_unpackedData.size(); a++)
		{
			m_top = m_unpackedData[a].coord.y > m_top.y ? m_unpackedData[a].coord : m_top;
			m_bottom = m_unpackedData[a].coord.y < m_bottom.y ? m_unpackedData[a].coord : m_bottom;
			m_left = m_unpackedData[a].coord.x < m_left.x ? m_unpackedData[a].coord : m_left;
			m_right = m_unpackedData[a].coord.x > m_right.x ? m_unpackedData[a].coord : m_right;
			m_front = m_unpackedData[a].coord.z < m_front.z ? m_unpackedData[a].coord : m_front;
			m_back = m_unpackedData[a].coord.z > m_back.z ? m_unpackedData[a].coord : m_back;
		}



	}

private:
	bool m_invert;
};

struct primitiveSphear:public primitiveMesh
{
public:
	primitiveSphear():primitiveMesh() { type = SPHEAR; }

	primitiveSphear(Coord2D<> dim, int segments, int divisions, Coord3D<> offset = {}, Coord3D<> rot = {}, bool invert = false):
		primitiveMesh(dim, offset, rot),
		m_segments(segments),
		m_divisions(divisions),
		m_invert(invert)
	{
		type = SPHEAR; createMesh();
	}

	primitiveSphear(float w, float h, int segments, int divisions, Coord3D<> offset = {}, Coord3D<> rot = {}, bool invert = false):
		primitiveMesh(w, h, w, offset, rot),
		m_segments(segments),
		m_divisions(divisions),
		m_invert(invert)
	{
		type = SPHEAR;  createMesh();
	}

	~primitiveSphear() {}

	void createMesh()
	{
		std::vector<Vertex3D>tmp;

		//calculate segments
		for(int b = 1; b <= m_divisions; ++b)
			for(int a = 0; a <= m_segments; ++a)
			{
				Quat tmp2;
				if(b == 1)
				{
					if(a % 2 == 0 || a > 1)
					{
						//Top Coord
						tmp.push_back({{0,m_dim.h * .5f,0}});
						tmp.back().norm = {0,1,0};

						//last segment
						if(tmp.size() > 3)
							tmp.push_back(tmp[tmp.size() - 2]);
					}
					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, (m_dim.h * .5f)* (1.f - (b / (float)m_divisions) * (b / (float)m_divisions)), (m_dim.w * .5f)* ((b / (float)m_divisions) / (b / (float)m_divisions))}).rotation(360.f / m_segments * a,0,1,0))});

					if(!(tmp.size() % 3) && tmp.size() > 2)
					{
						Coord3D<> norm = Coord3D<>::crossProduct(tmp[tmp.size() - 1].coord - tmp[tmp.size() - 3].coord, tmp[tmp.size() - 1].coord - tmp[tmp.size() - 2].coord).normal();
						tmp[tmp.size() - 1].norm =
							tmp[tmp.size() - 2].norm =
							tmp[tmp.size() - 3].norm = norm;

					}
				}
				else
				{

					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - ((b - 1) / (float)m_divisions) * ((b - 1) / (float)m_divisions)), m_dim.w * .5f * (((b - 1) / (float)m_divisions) / ((b - 1) / (float)m_divisions))}).rotation(360.f / m_segments * a,0,1,0))});
					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - (b / (float)m_divisions) * (b / (float)m_divisions)), m_dim.w * .5f * ((b / (float)m_divisions) / (b / (float)m_divisions))}).rotation(360.f / m_segments * a,0,1,0))});
					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - (b / (float)m_divisions) * (b / (float)m_divisions)), m_dim.w * .5f * ((b / (float)m_divisions) / (b / (float)m_divisions))}).rotation(360.f / m_segments * (a + 1),0,1,0))});

					Coord3D<> norm = Coord3D<>::crossProduct(tmp[tmp.size() - 1].coord - tmp[tmp.size() - 3].coord, tmp[tmp.size() - 1].coord - tmp[tmp.size() - 2].coord).normal();
					tmp[tmp.size() - 1].norm =
						tmp[tmp.size() - 2].norm =
						tmp[tmp.size() - 3].norm = norm;

					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - ((b - 1) / (float)m_divisions) * ((b - 1) / (float)m_divisions)), m_dim.w * .5f * (((b - 1) / (float)m_divisions) / ((b - 1) / (float)m_divisions))}).rotation(360.f / m_segments * a,0,1,0))});
					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - (b / (float)m_divisions) * (b / (float)m_divisions)), m_dim.w * .5f * ((b / (float)m_divisions) / (b / (float)m_divisions))}).rotation(360.f / m_segments * (a + 1),0,1,0))});
					tmp.push_back({reclass(Coord3D<>,tmp2 = Quat(Coord3D<> {0, m_dim.h * .5f * (1.f - ((b - 1) / (float)m_divisions) * ((b - 1) / (float)m_divisions)), m_dim.w * .5f * (((b - 1) / (float)m_divisions) / ((b - 1) / (float)m_divisions))}).rotation(360.f / m_segments * (a + 1),0,1,0))});

					norm = Coord3D<>::crossProduct(tmp[tmp.size() - 1].coord - tmp[tmp.size() - 3].coord, tmp[tmp.size() - 1].coord - tmp[tmp.size() - 2].coord).normal();
					tmp[tmp.size() - 1].norm =
						tmp[tmp.size() - 2].norm =
						tmp[tmp.size() - 3].norm = norm;
				}

			}

		indiceCreator(tmp);
	}
private:
	int m_segments = 1, m_divisions;
	bool m_invert;
};

class Mesh
{
public:
	Mesh();
	Mesh(Mesh& mesh);
	~Mesh();

	void init();

	bool loadPrimitive(primitiveMesh* mesh);


	//TODO: Migrate this to Model
	void editVerts(Mesh* verts1, Mesh* verts2);

	void render(Shader& shader);

	void unload();

	void replaceTexture(int index, GLuint tex);

	Coord3D<> top, bottom, left, right, front, back;
	std::string meshName;
	std::vector<std::string> matNames;

	std::vector<Vertex3D>& getUnpackedData() { return m_unpackedData; }
	std::vector<unsigned>& getIndicieData() { return m_indicieData; }
	std::vector<Texture2D>& getTextures() { return m_textures; }

	std::vector<GLuint>& getReplaceTex() { return m_replaceTex; }
private:


	std::vector<GLuint> m_replaceTex;

	bool ani;

	GLuint m_vaoID;
	std::pair<GLuint, GLuint> m_vboID;
	GLuint m_elemID;

	std::vector<Vertex3D> m_unpackedData;
	std::vector<unsigned> m_indicieData;
	std::vector<Texture2D>m_textures;


};

