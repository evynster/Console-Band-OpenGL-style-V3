#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <vector>
#include <map>
#include "Transformer.h"
#include "Shader.h"
#include "Utilities.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"

#define CHAR_BUFF_SIZE 1000

enum PRIMITIVE_TYPE
{
	NONE,
	PLANE,
	CUBE
};

struct primitiveMesh
{
	primitiveMesh() {}
	primitiveMesh(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}):m_dim(dim), m_offset(offset), m_rotation(rot) {}
	primitiveMesh(float width, float height, float depth, Coord3D<> offset = {}, Coord3D<> rot = {})
		:m_dim({width,height,depth}), m_offset(offset), m_rotation(rot) {}

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
	template<class _iter>
	void indiceAdder(_iter begin, _iter end, std::vector<Vertex3D> verts)
	{
		for(auto& i = begin; i < end; i++)
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

	PrimitivePlane(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}):primitiveMesh(dim, offset, rot) { type = PLANE; }

	PrimitivePlane(float w, float h, float d = 0, Coord3D<> offset = {}, Coord3D<> rot = {})
		:primitiveMesh(w, h, d, offset, rot)
	{
		type = PLANE;
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
		//bottom left tri
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW,-halfH, halfD}).toVec3(), 1)
			)
			});//bottom left
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW,-halfH,-halfD}).toVec3(), 1)
			)
			});//bottom right*
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW, halfH,-halfD}).toVec3(), 1)
			)
			});//top left*

		//top right tri
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW, halfH,-halfD}).toVec3(), 1)
			)
			});//top right
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{-halfW, halfH, halfD}).toVec3(), 1)
			)
			});//top left*
		tmp.push_back({m_offset + reclass(Coord3D<>,
			Quat::quatRotationMat(m_rotation.x,1,0,0) * Quat::quatRotationMat(m_rotation.y,0,1,0) * Quat::quatRotationMat(m_rotation.z,0,0,1) *
			glm::vec4((Coord3D<>{ halfW,-halfH, halfD}).toVec3(), 1)
			)
			});//bottom right*


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
		//auto valy = (norm) * (tmp[0].coord - tmp[2].coord).normal();

		val = ((tmp[0].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[0].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//bottom left 
		val = ((tmp[1].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[1].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//bottom right*
		val = ((tmp[2].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[2].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//top left*

		val = ((tmp[3].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[3].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//top right
		val = ((tmp[4].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[4].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//top left*
		val = ((tmp[5].coord - m_offset) + Coord3D<>{halfW, halfH, halfD});
		tmp[5].uv = {float((val * x).distance() > 0),float((val* y).distance() > 0)};//bottom right*


		indiceCreator(tmp);
	}
};

struct primitiveCube: public primitiveMesh
{
	primitiveCube():primitiveMesh() { type = CUBE; }

	primitiveCube(Coord3D<> dim, Coord3D<> offset = {}, Coord3D<> rot = {}):primitiveMesh(dim, offset, rot) { type = CUBE; }

	primitiveCube(float w, float h, float d = 0, Coord3D<> offset = {}, Coord3D<> rot = {})
		:primitiveMesh(w, h, d, offset, rot)
	{
		type = CUBE;
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

			a.coord = reclass(Coord3D<>,rot * glm::vec4(a.coord.toVec3(), 1));
			a.norm = reclass(Coord3D<>, rot * glm::vec4(a.norm.toVec3(), 1));
		}


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

};

class Mesh
{
public:
	Mesh();
	Mesh(Mesh& mesh);
	~Mesh();

	bool loadMesh(std::string);

	bool loadPrimitive(primitiveMesh* mesh);

	std::vector<std::vector<Vertex3D>>& loadAni(std::string);

	void editVerts(std::vector< std::vector<Vertex3D>> verts1, std::vector< std::vector<Vertex3D>> verts2);

	void render(Shader& shader);

	void unload();

	void replaceTexture(int mesh, int index, GLuint tex);

	Coord3D<> top, bottom, left, right, front, back;

private:
	void loadMaterials(const char* path);

	bool load(std::string path);

	void init();


	std::vector<std::vector<GLuint>> m_replaceTex;

	bool ani;

	std::vector<GLuint> m_vaoID;
	std::vector<std::pair<GLuint, GLuint>> m_vboID;
	std::vector<std::pair<std::string, GLuint>> m_elemID;

	//std::vector<Coord3D<>> m_verts;
	//std::vector<UV> m_uvs;
	//std::vector<Coord3D<>> m_norms;

	std::vector<std::vector<Vertex3D>> m_unpackedData;
	std::vector< std::pair<std::string, std::vector<unsigned>>> m_indicieData;
	std::vector<std::pair<std::string, std::vector<Texture2D>>>m_textures;

};

