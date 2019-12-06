#include "Mesh.h"
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

Mesh::Mesh()
{
	if(!fs::exists("Models/BIN"))
		system("mkdir \"Models/BIN\"");
}

Mesh::Mesh(Mesh& mesh):
	m_vaoID(mesh.m_vaoID),
	m_vboID(mesh.m_vboID),
	m_textures(mesh.m_textures),
	m_replaceTex(mesh.m_replaceTex),
	top(mesh.top),
	bottom(mesh.bottom),
	left(mesh.left),
	right(mesh.right),
	front(mesh.front),
	back(mesh.back)
{



	if(!fs::exists("Models/BIN"))
		system("mkdir \"Models/BIN\"");
}

Mesh::~Mesh()
{
#if _DEBUG
	printf("Deleted Mesh\n");
#endif // _DEBUG

	if(!ani)
		unload();

	m_textures.clear();
	m_replaceTex.clear();
}

std::string substr(const char* str, const char* find)
{
	char tmp[CHAR_BUFF_SIZE];
	unsigned count = (unsigned)strlen(str);
	for(; count >= 0; --count)
		if(strstr(str + count, find))
			break;

	memset(tmp, 0, CHAR_BUFF_SIZE);
	memmove_s(tmp, CHAR_BUFF_SIZE, str, count + 1);
	return std::string(tmp);
}

void Mesh::loadMaterials(const char* path)
{
	FILE* f;
	cDir((char*)path);
	fopen_s(&f, path, "r");
	if(!f)
	{
		printf("unknown material\n");
		return;
	}
	char str[CHAR_BUFF_SIZE];
	char* MeshCheck;
	std::string tmpDir;
	while(MeshCheck = fgets(str, CHAR_BUFF_SIZE, f),
		//this part takes out the '\n' from the string
		(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
		MeshCheck)
		if(strstr(str, "mtllib"))
		{
			//find material
			char str2[CHAR_BUFF_SIZE];
			memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

			//find material location
			char tmp[CHAR_BUFF_SIZE];
			unsigned count = unsigned(strrchr(path, '/') - path) + 1;
			memset(tmp, 0, CHAR_BUFF_SIZE);
			memmove_s(tmp, CHAR_BUFF_SIZE, path, count);
			tmpDir = std::string(tmp) + std::string(str2);

			//set material path
			path = tmpDir.c_str();
			break;
		}

	fclose(f);

	fopen_s(&f, path, "r");


	if(f)
		while(MeshCheck = fgets(str, CHAR_BUFF_SIZE, f),
			//this part takes out the '\n' from the string
			(str == nullptr ? "" : (str[strlen(str) - 1] = (str[strlen(str) - 1] == '\n' ? '\0' : str[strlen(str) - 1]), str)),
			MeshCheck)
		{

			if(strchr(str, '#'))
				memset(strchr(str, '#'), '\0', sizeof(char));

			if(strstr(str, "newmtl"))
			{
				char str2[CHAR_BUFF_SIZE];
				sscanf_s(str, "newmtl %s", str2, CHAR_BUFF_SIZE);
				m_textures.push_back({std::string(str2), std::vector<Texture2D>()});
				m_replaceTex.push_back(std::vector<GLuint>());
				if(strstr(str, "None"))
					return;
			}

			else if(strstr(str, "illum"))
			{
				continue;
			}
			else if(strstr(str, "map_Kd"))
			{
				char str2[CHAR_BUFF_SIZE];
				//sscanf_s(str, "map_Kd %s", &str2, (unsigned)_countof(str2));
				memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

				//path.resizeDepth(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::DIFFUSE;
				m_replaceTex.back().push_back(0);

			}
			else if(strstr(str, "map_Ks"))
			{
				char str2[CHAR_BUFF_SIZE];
				memcpy_s(str2, CHAR_BUFF_SIZE, str + 7, CHAR_BUFF_SIZE - 7);

				//path.resizeDepth(path.c_str());
				cDir(str2);
				std::string tmpStr(substr(path, "/") + str2);
				m_textures.back().second.push_back(ResourceManager::getTexture2D(tmpStr.c_str()));
				m_textures.back().second.back().type = TEXTURE_TYPE::SPECULAR;
				m_replaceTex.back().push_back(0);

			}
			else if(strstr(str, "Ns"))
			{
				continue;
			}
			else if(strstr(str, "Ka"))
			{
				float a[3];
				sscanf_s(str, "Ka %f %f %f", &a[0], &a[1], &a[2]);
				for(auto& b : m_textures.back().second)
					b.colour.a = (GLubyte)(255 * a[0] * a[1] * a[2]);
			}
			else if(strstr(str, "Kd"))
			{
				float r, g, b;
				sscanf_s(str, "Kd %f %f %f", &r, &g, &b);
				for(auto& a : m_textures.back().second)
					if(a.type == TEXTURE_TYPE::DIFFUSE)
						a.colour.set(r, g, b);
			}
			else if(strstr(str, "Ks"))
			{
				continue;
			}
			else if(strstr(str, "Ke"))
			{
				continue;
			}
			else if(strstr(str, "Ni"))
			{
				continue;
			}
			else if(strchr(str, 'd'))
			{
				continue;
			}
		}

	if(f)
		fclose(f);
}

bool Mesh::load(std::string path)
{
	if(!strstr(path.c_str(), ".obj"))return false;
	unload();


	//std::vector < std::pair<std::string, std::vector<Coord3D<unsigned>[3]>>> faces;
	FILE* bin;
	cDir((char*)path.c_str());

	//path.insert(path.begin(), '\"');
	//path.insert(path.end(), '\"');

	std::vector<Coord3D<>>verts;
	std::vector<UV>uvs;
	std::vector<Coord3D<>>norms;

	std::map<Indicie, GLuint> indicieMap;
	unsigned indicieCount = 0;

	bool initFace = true;
	if(!fs::exists((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin"))
	{
		FILE* f;

		fopen_s(&f, path.c_str(), "r");
		if(!f)
		{
			printf("unknown file\n");
			return false;
		}

		loadMaterials(path.c_str());


		char inputBuff[CHAR_BUFF_SIZE];


		char* MeshCheck = nullptr;
		while(MeshCheck = fgets(inputBuff, CHAR_BUFF_SIZE, f),
			//this part takes out the '\n' from the string
			(inputBuff == nullptr ? "" : (inputBuff[strlen(inputBuff) - 1] = (inputBuff[strlen(inputBuff) - 1] == '\n' ? ' ' : inputBuff[strlen(inputBuff) - 1]), inputBuff)),
			MeshCheck)
		{
			//checks for comments
			if(strchr(inputBuff, '#'))
				memset(strchr(inputBuff, '#'), '\0', sizeof(char));
			if(strstr(inputBuff, "mtllib"))

				continue;

			if(strstr(inputBuff, "usemtl"))
			{

				char str[CHAR_BUFF_SIZE];
				sscanf_s(inputBuff, "usemtl %s", str, CHAR_BUFF_SIZE);
				m_indicieData.push_back({std::string(str),std::vector<unsigned>()});
				m_unpackedData.push_back(std::vector<Vertex3D>());
				indicieMap.clear();
				indicieCount = 0;
			}
			else if(strstr(inputBuff, "vt"))
			{
				//UV data

				UV tmp;
				sscanf_s(inputBuff, "vt %f %f", &tmp.u, &tmp.v);
				uvs.push_back(tmp);
			}
			else if(strstr(inputBuff, "vn"))
			{
				//Normal data
				Coord3D<> tmp;
				sscanf_s(inputBuff, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				norms.push_back(tmp);
			}
			else if(strchr(inputBuff, 'o'))
				continue;
			else if(strchr(inputBuff, 's'))
				continue;
			else if(strchr(inputBuff, 'f'))//Collect Face Data
			{
				//Face data

				Indicie tmp[3];

				char check = 0;
				unsigned counter = 0, count = 0;
				while(bool(check = inputBuff[counter++]))
					count += (check == '/');

				count = unsigned((float)count * .5f);
				std::string	faceTmp[2][2]
				{{ " %d/%d/%d"," %*d/%*d/%*d" },
					{ " %d//%d"," %*d//%*d" }};

				std::vector<std::string > format = std::vector<std::string>(count);
				std::string formatStr;
				std::function<void(int)> reformat = [&](int type)
				{
					for(unsigned a = 0; a < count; a++)
						if(a < 3)//read 3 indicies
							format[a] = faceTmp[type][0];
						else
							format[a] = faceTmp[type][1];
				};
				short type = 0;
				reformat(type);
				formatStr = "f";

				for(unsigned a = 0; a < count; a++)
					formatStr += format[a];



				if(9 != sscanf_s(inputBuff, formatStr.c_str(),
					//Coord         UV        Normal
					&tmp[0][0], &tmp[0][1], &tmp[0][2],
					&tmp[1][0], &tmp[1][1], &tmp[1][2],
					&tmp[2][0], &tmp[2][1], &tmp[2][2]))
				{
					reformat(++type);
					formatStr = "f";
					for(unsigned a = 0; a < count; a++)
						formatStr += format[a];

					sscanf_s(inputBuff, formatStr.c_str(),
						//Coord        Normal
						&tmp[0][0], &tmp[0][2],
						&tmp[1][0], &tmp[1][2],
						&tmp[2][0], &tmp[2][2]);
				}


				for(unsigned b = 0; b < 3; ++b)
				{
					tmp[b].correct();
					auto thing = indicieMap.find(tmp[b]);

					if(thing == indicieMap.end())//new indicie
					{
						indicieMap[tmp[b]] = indicieCount;
						Vertex3D tmp2;
						tmp2.coord = verts[tmp[b].coord];
						tmp2.norm = norms[tmp[b].norm];
						if(tmp[b].uv != -1)
							tmp2.uv = uvs[tmp[b].uv];

						m_unpackedData.back().push_back(tmp2);
						m_indicieData.back().second.push_back(indicieCount++);
					}
					else//reacouring indicie
						m_indicieData.back().second.push_back(indicieMap[tmp[b]]);
				}

				//tmp.correct();
				//m_unpackedData.push_back(Vertex3D());
				//m_unpackedData.back().coord = verts[tmp[0][0]];
				//m_indicieData.back().second.push_back(tmp);

				for(unsigned a = 1; a < count - 2; a++)
				{
					formatStr = "f";
					swap(format[a], format[(a + 2)]);
					for(unsigned i = 0; i < count; i++)
						formatStr += format[i];
					if(type == 0)
						sscanf_s(inputBuff, formatStr.c_str(),
							//Coord         UV        Normal
							&tmp[0][0], &tmp[0][1], &tmp[0][2],
							&tmp[1][0], &tmp[1][1], &tmp[1][2],
							&tmp[2][0], &tmp[2][1], &tmp[2][2]);
					else
						sscanf_s(inputBuff, formatStr.c_str(),
							//Coord       Normal
							&tmp[0][0], &tmp[0][2],
							&tmp[1][0], &tmp[1][2],
							&tmp[2][0], &tmp[2][2]);
					for(unsigned b = 0; b < 3; ++b)
					{
						tmp[b].correct();
						auto thing = indicieMap.find(tmp[b]);

						if(thing == indicieMap.end())//new indicie
						{
							indicieMap[tmp[b]] = indicieCount;
							Vertex3D tmp2;
							tmp2.coord = verts[tmp[b].coord];
							tmp2.norm = norms[tmp[b].norm];
							if(tmp[b].uv != -1)
								tmp2.uv = uvs[tmp[b].uv];

							m_unpackedData.back().push_back(tmp2);
							m_indicieData.back().second.push_back(indicieCount++);
						}
						else//reacouring indicie
							m_indicieData.back().second.push_back(indicieMap[tmp[b]]);
					}


				}

			}
			else if(strchr(inputBuff, 'v'))//Collects Vertex Data
			{
				//Vertex Data

				Coord3D<> tmp;
				sscanf_s(inputBuff, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z);
				verts.push_back(tmp);
				if(initFace)
				{
					front = back = left = right = top = bottom = tmp;
					initFace = false;
				}
				else
				{
					front = tmp.z > front.z ? tmp : front;
					back = tmp.z < back.z ? tmp : back;
					left = tmp.x < left.x ? tmp : left;
					right = tmp.x > right.x ? tmp : right;
					top = tmp.y > top.y ? tmp : top;
					bottom = tmp.y < bottom.y ? tmp : bottom;
				}
			}

		}
		fclose(f);




		//UNPACKED DATA//
		{

			//open bin file
			fopen_s(&bin, ((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin").c_str(), "wb");

			unsigned unpackedSize, meshSize = m_indicieData.size(), dataSize = 0;

			//ammount of meshes
			fwrite(&meshSize, sizeof(char), sizeof(int), bin);

			for(unsigned int a = 0; a < meshSize; a++)
			{
				unpackedSize = m_unpackedData[a].size();
				//store vertex Data
				fwrite(&unpackedSize, 1, sizeof(unsigned), bin);
				fwrite(m_unpackedData[a].data(), 1, sizeof(Vertex3D) * unpackedSize, bin);


				dataSize = m_indicieData[a].second.size();
				int chars = (int)m_indicieData[a].first.size();
				//Length of string
				fwrite(&chars, sizeof(char), sizeof(int), bin);
				//Name of mesh
				fwrite(m_indicieData[a].first.c_str(), sizeof(char), sizeof(char) * chars, bin);
				//Amount of indicies 
				fwrite(&dataSize, sizeof(char), sizeof(unsigned int), bin);
				//Indicie data
				fwrite(m_indicieData[a].second.data(), 1, sizeof(unsigned) * dataSize, bin);
			}
		}
	}
	else
	{
		loadMaterials(path.c_str());

		//UNPACKED DATA//
		{
			//open bin file
			fopen_s(&bin, ((path.substr(0, path.find('/') + 1) + "BIN") + path.substr(path.find_last_of('/'), path.find_first_of('.') - path.find_last_of('/') + 1) + "bin").c_str(), "rb");

			unsigned unpackedSize = 0, meshSize = 0, dataSize = 0;

			//ammount of meshes
			fread(&meshSize, sizeof(char), sizeof(int), bin);

			for(unsigned int a = 0; a < meshSize; a++)
			{

				//store vertex Data
				fread(&unpackedSize, 1, sizeof(unsigned), bin);
				m_unpackedData.push_back(std::vector<Vertex3D>());
				m_unpackedData.back().resize(unpackedSize);
				fread(m_unpackedData[a].data(), 1, sizeof(Vertex3D) * unpackedSize, bin);


				dataSize = 0;
				int chars = 0;

				//Length of string
				fread(&chars, sizeof(char), sizeof(int), bin);

				m_indicieData.push_back({std::string(),std::vector<GLuint>()});
				m_indicieData.back().first.resize(chars);

				//Name of mesh
				fread(m_indicieData.back().first.data(), sizeof(char), sizeof(char) * chars, bin);

				//Amount of indicies 
				fread(&dataSize, sizeof(char), sizeof(unsigned int), bin);
				m_indicieData.back().second.resize(dataSize);

				//Indicie data
				fread(m_indicieData[a].second.data(), 1, sizeof(unsigned) * dataSize, bin);
			}

			for(auto& vec : m_unpackedData)
				for(auto& bound : vec)
					if(initFace)
					{
						front = back = left = right = top = bottom = bound.coord;
						initFace = false;
					}
					else
					{
						front = bound.coord.z > front.z ? bound.coord : front;
						back = bound.coord.z < back.z ? bound.coord : back;
						left = bound.coord.x < left.x ? bound.coord : left;
						right = bound.coord.x > right.x ? bound.coord : right;
						top = bound.coord.y > top.y ? bound.coord : top;
						bottom = bound.coord.y < bottom.y ? bound.coord : bottom;
					}
		}




	}

	if(bin)
		fclose(bin);

	return true;
}

bool Mesh::loadMesh(std::string path)
{
	if(!load(path.c_str()))return false;

	init();


	//for(auto& a : m_indicieData)
	//	a.second.clear();
	//m_indicieData.clear();

	return true;
}

bool Mesh::loadPrimitive(primitiveMesh* mesh)
{

	if(!mesh)
		return false;
	unload();

	mesh->createMesh();
	m_unpackedData.push_back(mesh->getData());
	m_indicieData.push_back({"",mesh->getIndices()});
	m_textures.resize(1, {"", {Texture2D(),Texture2D()}});
	m_replaceTex.resize(1, {{0,0}});

	m_textures[0].second[0].type = TEXTURE_TYPE::DIFFUSE;
	m_textures[0].second[1].type = TEXTURE_TYPE::SPECULAR;

	top = mesh->m_top;
	bottom = mesh->m_bottom;
	left = mesh->m_left;
	right = mesh->m_right;
	front = mesh->m_front;
	back = mesh->m_back;

	
	init();

	return true;
}

std::vector<std::vector<Vertex3D>>& Mesh::loadAni(std::string path)
{
	ani = true;
	static std::vector<std::vector<Vertex3D>> empty;

	if(!load(path))return empty;

	return m_unpackedData;

}

void Mesh::render(Shader& shader)
{
	shader.enable();
	for(unsigned a = 0; a < m_elemID.size(); a++)
	{
		bool textured = false;
		int c = 0;

		for(unsigned b = 0; b < m_textures.size(); b++)
		{
			if(m_textures[b].first == m_elemID[a].first)
			{
				glActiveTexture(GL_TEXTURE0 + c);
				int e = 0;
				for(auto& d : m_textures[b].second)
					if(d.type == TEXTURE_TYPE::DIFFUSE)
						if(d.id || m_replaceTex[b][e])
						{
							textured = true;
							glUniform1i(shader.getUniformLocation("uTex"), c);
							glBindTexture(GL_TEXTURE_2D, m_replaceTex[b][e] ? m_replaceTex[b][e] : d.id);
							e++;
						}
				c++;
			}
		}

		glUniform1i(shader.getUniformLocation("textured"), textured);


		glBindVertexArray(m_vaoID[a]);
		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elemID[a].second);

		//Draw indicies
		glDrawElements(
			GL_TRIANGLES,      // mode
			m_indicieData[a].second.size(),    // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);
		//glDrawArrays(GL_TRIANGLES, 0, m_numVerts[a]);
		glBindVertexArray(0);

		for(; c >= 0; c--)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D, 0);
			//glActiveTexture(0);
		}
	}

	shader.disable();
}

void Mesh::init()
{
	for(unsigned a = 0; a < m_indicieData.size(); a++)
	{
		m_vaoID.push_back(0);
		m_vboID.push_back({0,0});

		glGenVertexArrays(1, &m_vaoID.back());
		glGenBuffers(2, &m_vboID.back().first);

		glBindVertexArray(m_vaoID.back());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		//UnpackedData 1
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID.back().first);
		glBufferData(GL_ARRAY_BUFFER, m_unpackedData[a].size() * sizeof(Vertex3D), m_unpackedData[a].data(), GL_STATIC_DRAW);

		//vertex 1   attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));
		//UV         attributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
		//normal 1   attributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

		//UnpackedData 2 
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID.back().second);
		glBufferData(GL_ARRAY_BUFFER, m_unpackedData[a].size() * sizeof(Vertex3D), m_unpackedData[a].data(), GL_STATIC_DRAW);

		//vertex 2   attributes
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));
		//normals 2   attributes
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

		//create indicies
		m_elemID.push_back({m_indicieData[a].first,0});
		glGenBuffers(1, &m_elemID[a].second);

		//indicies 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elemID[a].second);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicieData[a].second.size() * sizeof(unsigned), m_indicieData[a].second.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::replaceTexture(int mesh, int index, GLuint tex)
{
	m_replaceTex[mesh][index] = tex;
}

void Mesh::editVerts(std::vector<  std::vector<Vertex3D>> verts1, std::vector<  std::vector<Vertex3D>> verts2)
{
	for(unsigned a = 0; a < m_indicieData.size(); a++)
	{

		//m_vaoID.push_back(0);
		//m_vboID.push_back({0,0});
		//
		//glGenVertexArrays(1, &m_vaoID.back());
		//glGenBuffers(2, &m_vboID.back().first);

		glBindVertexArray(m_vaoID[a]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		//UnpackedData 1
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].first);
		glBufferData(GL_ARRAY_BUFFER, verts1[a].size() * sizeof(Vertex3D), verts1[a].data(), GL_STATIC_DRAW);

		//vertex 1   attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));
		//UV         attributes
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));
		//normal 1   attributes
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

		//UnpackedData 2 
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID[a].second);
		glBufferData(GL_ARRAY_BUFFER, verts2[a].size() * sizeof(Vertex3D), verts2[a].data(), GL_STATIC_DRAW);

		//vertex 2   attributes
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, coord));
		//normals 2   attributes
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, norm));

		////create indicies
		//m_elemID.push_back({m_indicieData[a].first,0});
		//glGenBuffers(1, &m_elemID[a].second);

		//indicies 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elemID[a].second);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicieData[a].second.size() * sizeof(Indicie), m_indicieData[a].second.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::unload()
{
	for(unsigned a = 0; a < m_vaoID.size(); ++a)
	{
		if(m_vaoID[a])
			glDeleteVertexArrays(1, &m_vaoID[a]);

		if(m_vboID[a].first)
			glDeleteBuffers(2, &m_vboID[a].first);


		if(m_elemID[a].second)
			glDeleteBuffers(1, &m_elemID[a].second);
	}

	m_vaoID.clear();
	m_vboID.clear();
	m_elemID.clear();
	
	m_indicieData.clear();
	m_unpackedData.clear();
}