#include <GameEmGine.h>
#include "Song.h"
class Test: public Scene
{
public:
	bool moveLeft, moveRight, moveForward, moveBack, moveUp, moveDown,
		rotLeft, rotRight, rotUp, rotDown,
		tab = false;

	void init()
	{
		Game::setCameraPosition({0,0,-0});
		FrustumPeramiters peram{75,(float)Game::getWindowWidth() / Game::getWindowHeight(),0,100};
		Game::setCameraType(FRUSTUM, &peram);


		model[0] = new Model("Models/note/note.obj", "Box1");

		model[0]->setScale(1);
		testText.setText("Maybe this Works?");
		testText.setColour(1, .5, 0);
		testText.textSize(1);
		testText.
		
		model[1] = new Model(*model[0], "Box2");
		model[2] = new Model(*model[1], "Box3");

		//model[0]->addChild(model[1]);
		////model[0]->addChild(&testText);
		Game::addText(&testText);
		//model[1]->addChild(model[2]);


		Game::getMainCamera()->enableFPS();


		//model[0]->setScale(1);
		//model2->setScale(1);

		//model[0]->enableBoundingBox(true);
		//model2->enableBoundingBox(true);

		//Game::addModel(model2);
		Game::addModel(model[0]);
		//Game::addText(&testText);

		keyPressed =
			[&](int key, int mod)->void
		{
			if(key == GLFW_KEY_R)
			{
				Game::getMainCamera()->getTransformer().reset();
				Game::setCameraPosition({0,0,-5});
			}

			if(key == GLFW_KEY_F5)
				Shader::refresh();

			//static int count;
			if(key == GLFW_KEY_TAB)
				tab = !tab;//	std::swap(model[0], model[count++]);
			//count = count % 2 + 1;

			static bool fps = 0;
			if(key == 'F')
				model[0]->enableFPS(fps = !fps);

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


			if(key == GLFW_KEY_LEFT)
				rotLeft = false;

			if(key == GLFW_KEY_RIGHT)
				rotRight = false;

			if(key == GLFW_KEY_UP)
				rotUp = false;

			if(key == GLFW_KEY_DOWN)
				rotDown = false;
		};
		testText.toFramebufferTexture();
		model[0]->replaceTexture(0, 0, testText.getFramebuffer()->getColorHandle(0));

	}

	void cameraMovement()
	{
		// Movement
		if(moveLeft)
			Game::translateCameraBy({-.05f,0,0});
		if(moveRight)
			Game::translateCameraBy({.05f,0,0});
		if(moveForward)
			Game::translateCameraBy({0,0,.05f});
		if(moveBack)
			Game::translateCameraBy({0,0,-.05f});
		if(moveUp)
			Game::translateCameraBy({0,.05f,0});
		if(moveDown)
			Game::translateCameraBy({0,-.05f,0});

		// Rotation
		if(rotLeft)
			Game::RotateCameraBy(1, {0,-.5f,0});
		if(rotRight)
			Game::RotateCameraBy(1, {0,.5f,0});
		if(rotUp)
			Game::RotateCameraBy(1, {.5f,0,0});
		if(rotDown)
			Game::RotateCameraBy(1, {-.5f,0,0});
	}

	void update(double dt)
	{
	
		if(tab)
		{
			// Movement
			if(moveLeft)
				model[0]->translateBy({-.05f,0.f,0.f});
			if(moveRight)
				model[0]->translateBy({.05f,0,0});
			if(moveForward)
				model[0]->translateBy({0,0,.05f});
			if(moveBack)
				model[0]->translateBy({0,0,-.05f});
			if(moveUp)
				model[0]->translateBy({0,.05f,0});
			if(moveDown)
				model[0]->translateBy({0,-.05f,0});

			// Rotation
			if(rotLeft)
				model[0]->rotateBy({0,-5,0});
			if(rotRight)
				model[0]->rotateBy({0,5,0});
			if(rotUp)
				model[0]->rotateBy({5,0,0});
			if(rotDown)
				model[0]->rotateBy({-5,0,0});
		}
		else
		{
			cameraMovement();
		}

		model[0]->print();
		puts("");

		printf(model[0]->collision2D(model[1], {0,1,0}) ? "collision\n" : "no collision\n");
		puts("\n");

	}
private:

	Model* model[3];
	Text testText;
};



int main()
{
	Game::init("Window Band (Previously Console Band) V3", 800, 400);

	Test test;
	Song song;
	Game::setScene(&test);
	Game::run();

	return 0;
}