#pragma once
#include "Mesh.h"

class MeshLoader
{
public:
	static std::vector<Mesh*> loadMesh(std::string path);

private:
	static void loadMaterials(cstring path);

	static bool load(std::string path);

	static void cleanup();

	static std::vector<Mesh*> m_meshes;
	static std::vector<std::pair<std::string, std::vector<Texture2D>>> m_textures;
	//static std::vector<std::pair<std::string, std::vector<unsigned>>> m_indicieData;
	//static std::vector<std::vector<Vertex3D>> m_unpackedData;
	//static std::vector<std::pair<std::string,std::vector<Texture2D>>> m_textures;
	//static std::vector<std::vector<GLuint>> m_replaceTex;

};

