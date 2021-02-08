#pragma once
#include <functional>
#include "SkyBox.h"

/*
Tricking the  system to think the
GameEmgine class is created
*/
class Scene
{
public:
	virtual ~Scene() { m_parent = nullptr; }

	////shader initialization
	//virtual void shaderInit() = 0;

	virtual void init() = 0;

	//updates within game loop
	virtual void update(double dt) = 0;

	//the post buffer will be blitted at the end of the function call to the main buffer
	std::function<void(FrameBuffer* gbuff, FrameBuffer* postBuff)>  customPostEffect;

	void setParent(const Scene* parent) { m_parent =(Scene*) parent; };
	const Scene* getParent() { return m_parent; }
	const SkyBox& getSkyBox() { return m_skybox; }
	bool skyBoxEnabled = false;
	
	std::function<void(void)>render;

	std::function<void(int state, int button, int mod)>
		keyInput,
		mouseInput;

		//instance key is pressed 
	std::function<void(int key, int mod)>
		keyPressed;

		//instance key is released
	std::function<void(int key, int mod)>
		keyReleased;

		//instance button is pressed 
	std::function<void(int button, int mod)>
		mousePressed;

		//instance button is released
	std::function<void(int button, int mod)>
		mouseReleased;

protected:
	void enableSkyBox(bool enable) { skyBoxEnabled = enable; }
	
	void setSkyBox(cstring path) { m_skybox.setCubeMap(path); }
	

	SkyBox m_skybox;
	Scene* m_parent = nullptr;

};

