#pragma once
#include <vector>
//#include "ResourceManager.h"
#include "Transformer.h"
#include "Model.h"
#include "Camera.h"



struct Light :public Transformer
{
	enum TYPE
	{
		DEFAULT,
		POINT,
		DIRECTIONAL,
		SPOTLIGHT
	};

	void setLightType(Light::TYPE type);

	void setDiffuse(ColourRGBA diff);

	void setSpecular(ColourRGBA spec);

	void setSpecularExponent(float specEx);

	void setAttenuationConstant(float attenConst);

	void setAttenuationLinear(float attenLinear);

	void setAttenuationQuadratic(float attenQuad);

	void enableLight(bool enable);

//private: //maybe later 
	Light::TYPE type;

	ColourRGBA diffuse, specular;
	Coord3D<> direction = { 0,0,1 };
	float
		angleConstraint = 45,
		specularExponent = .1f,
		attenuationConst =.1f,
		attenuationLinear =.1f,
		attenuationQuadratic = .1f;

	bool enable = true;

};

class LightManager
{
public:

	static void addLight(Light* lit);
	static void removeLight(Light* lit);
	static void removeLight(unsigned index);


	static Light* getLight(unsigned index);

	static void setSceneAmbient(ColourRGBA ambi);

	static void setCamera(Camera* cam);

	static void setShader(Shader* shad);

	static std::vector<FrameBuffer*> shadowBuffers(unsigned w, unsigned h, std::map<void*, Model*>&, unsigned index);

	static void setFramebuffer(FrameBuffer* buff);

	static void update();
	static void clear();
private:
	//Coord3D m_coord, m_spec;
	static ColourRGBA m_ambient;
	static std::vector<Light*> m_lights;
	static FrameBuffer* m_framebuffer;
	static std::vector<std::vector<FrameBuffer*>>m_shadows;
	//static unsigned m_size;

	static Shader* m_shader;
	static Camera* m_cam;
	//static LightInfo m_info;

};

