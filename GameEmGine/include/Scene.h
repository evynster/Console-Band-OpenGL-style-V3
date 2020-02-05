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
	virtual ~Scene() { parent = nullptr; }

	////shader initialization
	//virtual void shaderInit() = 0;

	//updates within game loop
	virtual void update(double dt) = 0;

	virtual void init() = 0;

	std::function<void(void)>render;

	std::function<void(int state, int button, int mod)>
		keyInput,
		mouseInput;

	std::function<void(int button, int mod)>
		//instance key is pressed 
		keyPressed,

		//instance key is released
		keyReleased,

		//instance button is pressed 
		mousePressed,

		//instance button is released
		mouseReleased;
	void enableSkyBox(bool enable) { skyBoxEnabled = enable; }
	void setSkyBox(cstring path) { skybox.setCubeMap(path); }
	SkyBox skybox;
	Scene* parent = nullptr;
	bool skyBoxEnabled = false;
private:

};

