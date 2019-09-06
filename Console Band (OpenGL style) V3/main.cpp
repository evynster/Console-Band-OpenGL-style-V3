#include <GameEmGine.h>
#include "Song.h"
class Test: public Scene
{
public:
	bool moveLeft, moveRight, moveForward, moveBack, moveUp, moveDown,
		rotLeft, rotRight, rotUp, rotDown, tiltLeft, tiltRight,
		tab = false;

	void init()
	{
		Game::setCameraPosition({0,0,-2});
		FrustumPeramiters frustum{55,(float)Game::getWindowWidth() / Game::getWindowHeight(),0,100};
		Game::setCameraType(FRUSTUM, &frustum);


		//model[0] = new Model("Models/controls/controller.obj", "Box1");
		model[0] = new Model(new primitiveCube({5,5,5}), "Box1");

		model[0]->setScale(.01);
		testText.setText("Maybe this Works?");
		testText.setColour(1, .5, 0);
		testText.textSize(1);


		model[1] = new Model(*model[0], "Box2");
		model[2] = new Model(*model[1], "Box3");

		Game::addText(&testText);

		Game::getMainCamera()->enableFPSMode();

		Game::addModel(model[0]);

		keyPressed =
			[&](int key, int mod)->void
		{
			if(key == GLFW_KEY_R)
			{
				Game::getMainCamera()->reset();
				Game::setCameraPosition({0,0,-5});
			}

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

		testText.toFramebufferTexture(200);
		model[0]->replaceTexture(0, 0, testText.getTexture());
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


		if(Game::mouseCollision(model[0]))
			model[0]->setColour(0, 0, 1);
		else
			model[0]->setColour(1, 0, 0);
puts("\n");

	}
private:
	float speed = 0.01, angle = 1;

	Model* model[3];
	Text testText;
};



int main()
{
	Game::init("Window Band (Previously Console Band) V3", 800, 400);

	Test test;
	Song song;
	Game::setScene(&song);
	Game::run();

	return 0;
}