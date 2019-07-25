#include <GameEmGine.h>

class Test: public Scene
{
	bool moveLeft, moveRight, moveForward, moveBack,
		rotLeft, rotRight, rotUp, rotDown;
	void init()
	{
		GameEmGine::setCameraPosition({0,0,-5});

		model = new Model("Models/SmallRobot.obj");
		//model->getTransformer().setScale(10);
		model->enableBoundingBox(true);
		model->getTransformer().setScale(0.4f, 0.6f, 0.4f);
		GameEmGine::addModel(model);
		keyPressed =
			[&](int key, int mod)->void
		{
			if(key == 'A')
				moveLeft = true;

			if(key == 'D')
				moveRight = true;

			if(key == 'W')
				moveForward = true;


			if(key == 'S')
				moveBack = true;



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



			if(key == GLFW_KEY_LEFT)
				rotLeft = false;

			if(key == GLFW_KEY_RIGHT)
				rotRight = false;

			if(key == GLFW_KEY_UP)
				rotUp = false;

			if(key == GLFW_KEY_DOWN)
				rotDown = false;
		};

	}

	void update(double dt)
	{

		if(moveLeft)
			GameEmGine::moveCameraPositionBy({-.05,0,0});

		if(moveRight)
			GameEmGine::moveCameraPositionBy({.05,0,0});

		if(moveForward)
			GameEmGine::moveCameraPositionBy({0,0,.05});

		if(moveBack)
			GameEmGine::moveCameraPositionBy({0,0,-.05});

		if(rotLeft)
			model->getTransformer().rotateBy({0,-5,0});
		if(rotRight)
			model->getTransformer().rotateBy({0,5,0});
		if(rotUp)
			model->getTransformer().rotateBy({5,0,0});
		if(rotDown)
			model->getTransformer().rotateBy({-5,0,0});
	}

	Model* model;
};

int main()
{
	Test test;
	GameEmGine::init("Window Band (Previously Console Band) V3", 800, 400);
	GameEmGine::setScene(&test);
	GameEmGine::run();

	return 0;
}