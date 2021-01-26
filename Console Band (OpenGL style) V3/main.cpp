//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <GameEmGine.h>
#include "Song.h"
#include "Menu.h"

class Test: public Scene
{
public:
	bool moveLeft, moveRight, moveForward, moveBack, moveUp, moveDown,
		rotLeft, rotRight, rotUp, rotDown, tiltLeft, tiltRight,
		tab = false;

	void init()
	{
		Game::setBackgroundColour(0, 0, 0);
		Game::setCameraPosition({0,0,-3});
		FrustumPeramiters frustum{55,(float)Game::getWindowWidth() / Game::getWindowHeight(),0.001f,500};
		Game::setCameraType(Camera::FRUSTUM, &frustum);
		//Game::setBackgroundColour(.2, .2, 0);

		setSkyBox("Skyboxes/skybox/");
		enableSkyBox(true);
		model[0] = new Model(/*"Models/nanosuit/nanosuit.obj"*//*"Models/Note/note.obj"*//*"Models/BOSS/slam/bsl1.obj"*/new primitiveSphear(2, 2,19,20), "Box1");
		model[0]->setColour(.6, .13, .05);

		static Animation ani;
		ani.setAnimationSpeed(1);
		ani.addDir("Models/BOSS/slam/");
		//model[0]->addAnimation("IDK", &ani);

		model[0]->setAnimation("IDK");
		ani.play();
		ani.repeat(true);
		
		model[0]->setScale(1);
		//model[0]->setColour(1, .2, .2, 1);
		model[0]->setTransparent(false);
		//model[0]->setTransparent(false);

		lit.setLightType(Light::TYPE::POINT);
		lit.setParent(Game::getMainCamera());
		lit.setSpecular({0,0,145});
		
		LightManager::addLight(&lit);

		//testText.setText("Maybe this Works?");
		//testText.setColour(1, 0, 0);
		//testText.textSize(20);
		//testText.toTexture(50);		

		model[1] = new Model(*model[0], "Box2");
		model[2] = new Model(*model[1], "Box3");

		Game::addModel(model[1]);
		Game::addModel(model[2]);

		model[1]->translate(5, 0, 0);
		model[2]->translate(-10, 0, 0);

		//Game::addText(&testText);

		Game::getMainCamera()->enableFPSMode();
		Game::addModel(model[0]);

		forward = new Model(new primitiveCube({1, 1, 1}));
		forward->setParent(Game::getMainCamera());
		forward->setScale(.1);
		//Game::addModel(forward);
		forward->translate(Game::getMainCamera()->getForward() );
		//model[0]->replaceTexture(0, 0, testText.getTexture());

		keyPressed =
			[&](int key, int mod)->void
		{
			if(key == 'R')
			{
				Game::getMainCamera()->reset();
				Game::setCameraPosition({0,0,-3});
			}
			static bool sky = true , frame = false;
			if(key == 'N')
				model[0]->setWireframe(frame=!frame);
			if(key == GLFW_KEY_SPACE)
				enableSkyBox(sky = !sky);

			if(key == GLFW_KEY_F5)
				Shader::refresh();

			//static int count;
			if(key == GLFW_KEY_TAB)
				tab = !tab;//	std::swap(model[0], model[count++]);

			static bool fps = 0;
			if(key == 'F')
				model[0]->enableFPSMode(fps = !fps);

			if(key == 'A')
				moveLeft = true;

			if(key == 'D')
				moveRight = true;

			if(key == 'W')
				moveForward = true;

			if(key == 'S')
				moveBack = true;

			if(key == 'Q')
				moveDown = true;

			if(key == 'E')
				moveUp = true;


			if(key == GLFW_KEY_PAGE_UP)
				tiltLeft = true;

			if(key == GLFW_KEY_PAGE_DOWN)
				tiltRight = true;

			if(key == GLFW_KEY_LEFT)
				rotLeft = true;

			if(key == GLFW_KEY_RIGHT)
				rotRight = true;

			if(key == GLFW_KEY_UP)
				rotUp = true;

			if(key == GLFW_KEY_DOWN)
				rotDown = true;
		};

		keyReleased =
			[&](int key, int mod)->void
		{
			if(key == 'A')
				moveLeft = false;

			if(key == 'D')
				moveRight = false;

			if(key == 'W')
				moveForward = false;

			if(key == 'S')
				moveBack = false;

			if(key == 'Q')
				moveDown = false;

			if(key == 'E')
				moveUp = false;


			if(key == GLFW_KEY_PAGE_UP)
				tiltLeft = false;

			if(key == GLFW_KEY_PAGE_DOWN)
				tiltRight = false;

			if(key == GLFW_KEY_LEFT)
				rotLeft = false;

			if(key == GLFW_KEY_RIGHT)
				rotRight = false;

			if(key == GLFW_KEY_UP)
				rotUp = false;

			if(key == GLFW_KEY_DOWN)
				rotDown = false;
		};
		auto list = Component::getComponentList();
		printf("there are %d items in list",list.size());
	}

	void cameraMovement()
	{
		// Movement
		if(moveLeft)
			Game::translateCameraBy({-speed,0,0});
		if(moveRight)
			Game::translateCameraBy({speed,0,0});
		if(moveForward)
			Game::translateCameraBy({0,0,speed});
		if(moveBack)
			Game::translateCameraBy({0,0,-speed});
		if(moveUp)
			Game::translateCameraBy({0,speed,0});
		if(moveDown)
			Game::translateCameraBy({0,-speed,0});

		// Rotation
		if(tiltLeft)
			Game::rotateCameraBy({0,0,-angle});
		if(tiltRight)
			Game::rotateCameraBy({0,0,angle});
		if(rotLeft)
			Game::rotateCameraBy({0,-angle,0});
		if(rotRight)
			Game::rotateCameraBy({0,angle,0});
		if(rotUp)
			Game::rotateCameraBy({angle,0,0});
		if(rotDown)
			Game::rotateCameraBy({-angle,0,0});
	}

	void objectMovement()
	{
		// Movement
		if(moveLeft)
			model[0]->translateBy({-speed,0.f,0.f});
		if(moveRight)
			model[0]->translateBy({speed,0,0});
		if(moveForward)
			model[0]->translateBy({0,0,speed});
		if(moveBack)
			model[0]->translateBy({0,0,-speed});
		if(moveUp)
			model[0]->translateBy({0,speed,0});
		if(moveDown)
			model[0]->translateBy({0,-speed,0});

		// Rotation
		if(rotLeft)
			model[0]->rotateBy({0,-angle,0});
		if(rotRight)
			model[0]->rotateBy({0,angle,0});
		if(rotDown)
			model[0]->rotateBy({-angle,0,0});
		if(rotUp)
			model[0]->rotateBy({angle,0,0});

		
	}

	void update(double dt)
	{
		if(tab)
			objectMovement();
		else
			cameraMovement();

	}
private:
	float speed = 0.1f, angle = 1;

	Model* model[3];
	Model* forward;
	Text testText;
	Light lit;

};

int main()
{

	Game::init("Window Band (Previously Console Band) V3", 900, 500);


	Test test;
	//Song song;
	Menu main;
	Game::setScene(&test);
	Game::run();

	return 0;
}