//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include <GameEmGine.h>
#include "Song.h"
#include "Menu.h"

class Test: public Scene
{

#pragma region Variables

	float speed = 0.1f, angle = 1;
	Animation ani;
	Model* model[10];
	Model* bigBoss[2];
	Model* rocket;

	Model* forward;
	Text testText;
	Light lit;
#pragma endregion

public:
	bool moveLeft, moveRight, moveForward, moveBack, moveUp, moveDown,
		rotLeft, rotRight, rotUp, rotDown, tiltLeft, tiltRight,
		tab = false;

	void init()
	{
		Game::setBackgroundColour(.15, .15, .15);
		Game::setCameraPosition({0,0,-3});
		FrustumPeramiters frustum{65,(float)Game::getWindowWidth() / Game::getWindowHeight(),0.001f,500};
		Game::setCameraType(&frustum);

		setSkyBox("Skyboxes/space/");
		enableSkyBox(true);




		// Scene setup 
		bigBoss[0] = new Model("Models/BOSS/slam/bsl1.obj", "Boss1");
		bigBoss[0]->translate(-15, 0, 0);

		ani.setAnimationSpeed(1);
		ani.addDir("Models/BOSS/slam/");
		bigBoss[0]->addAnimation("IDK", &ani);

		bigBoss[0]->setAnimation("IDK");
		ani.play();
		ani.repeat(true);


		rocket = new Model("Models/rocket-ship/rocket ship.obj", "ship");
		bigBoss[1] = new Model(*bigBoss[0], "Boss2");
		bigBoss[1]->translate(15, 0, 0);
		bigBoss[0]->rotate(0, 90, 0);
		bigBoss[1]->rotate(0, -90, 0);

		//floor 
		static Model floor = Model(new PrimitivePlane({100,0,100}), "floor");
		Game::addModel(&floor);
		floor.replaceTexture(0, 0, ResourceManager::getTexture2D("Textures/moon.jpg").id);



		lit.setLightType(Light::TYPE::POINT);
		lit.setParent(Game::getMainCamera());
		lit.setSpecular({0,0,145});

		LightManager::addLight(&lit);

		//testText.setText("Maybe this Works?");
		//testText.setColour(1, 0, 0);
		//testText.textSize(20);
		//testText.toTexture(50);		

		Game::addModel(bigBoss[0]);
		Game::addModel(bigBoss[1]);


		//Game::addText(&testText);

		Game::getMainCamera()->enableFPSMode();
		Game::addModel(rocket);

		//forward.translate(Game::getMainCamera()->getForward());
		//model[0].replaceTexture(0, 0, testText.getTexture());

		keyPressed =
			[&](int key, int mod)->void
		{

			switch(key)
			{
			case GLFW_KEY_1:
				lit.enableDiffuse(false);
				lit.enableSpecular(false);
				Game::enableBloom(false);
				break;
			case GLFW_KEY_2:
				lit.enableDiffuse(true);
				lit.enableSpecular(false);
				Game::enableBloom(false);
				break;
			case GLFW_KEY_3:
				lit.enableDiffuse(false);
				lit.enableSpecular(true);
				Game::enableBloom(false);
				break;
			case GLFW_KEY_4:
				lit.enableDiffuse(true);
				lit.enableSpecular(true);
				Game::enableBloom(false);
				break;
			case GLFW_KEY_5:
				lit.enableDiffuse(true);
				lit.enableSpecular(true);
				Game::enableBloom(true);
				break;
			}


			if(key == 'R')
			{
				Game::getMainCamera()->reset();
				Game::setCameraPosition({0,0,-3});
			}
			static bool sky = true, frame = false;
			if(key == 'N')
				model[0]->setWireframe(frame = !frame);
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
		printf("there are %d items in the list", list.size());
		puts("here they are: ");
		for(auto& a : list)
			for(int b = 0; b < a.second; b++)
				puts(a.first.c_str());
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
			bigBoss[0]->translateBy({-speed,0.f,0.f});
		if(moveRight)
			bigBoss[0]->translateBy({speed,0,0});
		if(moveForward)
			bigBoss[0]->translateBy({0,0,speed});
		if(moveBack)
			bigBoss[0]->translateBy({0,0,-speed});
		if(moveUp)
			bigBoss[0]->translateBy({0,speed,0});
		if(moveDown)
			bigBoss[0]->translateBy({0,-speed,0});

		// Rotation
		if(rotLeft)
			bigBoss[0]->rotateBy({0,-angle,0});
		if(rotRight)
			bigBoss[0]->rotateBy({0,angle,0});
		if(rotDown)
			bigBoss[0]->rotateBy({-angle,0,0});
		if(rotUp)
			bigBoss[0]->rotateBy({angle,0,0});


	}

	void update(double dt)
	{
		cameraMovement();
		int speed = 5;
		ani.setAnimationSpeed((float)speed / ani.getTotalFrames());
		bigBoss[0]->translate(lerp(Coord3D<>{-60, 0, 0}, Coord3D<>{-15, 0, 0}, fmod(clock() / (float)CLOCKS_PER_SEC-1, speed)/speed));
		bigBoss[1]->translate(lerp(Coord3D<>{60, 0, 0}, Coord3D<>{15, 0, 0}, fmod(clock() / (float)CLOCKS_PER_SEC-1, speed)/speed));
	}



};

int main()
{
	Game::init("Assignment 1", 900, 500);

	Test test;
	Song song;
	Game::setScene(&song);
	Game::run();

	return 0;
}