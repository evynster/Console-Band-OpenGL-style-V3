#include <GameEmGine.h>

class Test: public Scene
{
public:
	bool moveLeft, moveRight, moveForward, moveBack,moveUp,moveDown,
		rotLeft, rotRight, rotUp, rotDown,
		tab = false;

	void init()
	{
		GameEmGine::setCameraPosition({0,0,-5});

		model1 = new Model("Models/SmallRobot.obj","Box1");
		model2 = new Model(*model1,  "Box2");

		

		testText.setText("MAybe this Works?");
		testText.textSize(3);
		ColourRGBA col; col.set(1.f,0,0);
		testText.textColour(col);

		GameEmGine::getMainCamera()->enableFPS();
		model1->enableFPS();

		model1->setScale(2);
		model2->setScale(1);

		model1->enableBoundingBox(true);
		model2->enableBoundingBox(true);

		GameEmGine::addModel(model1);
		GameEmGine::addModel(model2);
		GameEmGine::addText(&testText);

		keyPressed =
			[&](int key, int mod)->void
		{
			if(key == GLFW_KEY_R)
			{
				GameEmGine::getMainCamera()->getTransformer().reset();
				GameEmGine::setCameraPosition({0,0,-5});
			}

			if(key == GLFW_KEY_F5)
				Shader::refresh();

			if(key == GLFW_KEY_TAB)
				tab = !tab;			

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

	}

	void update(double dt)
	{
		if(!tab)
		{
			// Movement
			if(moveLeft)
				model1->translateBy({-.05f,0.f,0.f});
			if(moveRight)
				model1->translateBy({.05f,0,0});
			if(moveForward)
				model1->translateBy({0,0,.05f});
			if(moveBack)
				model1->translateBy({0,0,-.05f});
			if(moveUp)
				model1->translateBy({0,.05f,0});
			if(moveDown)
				model1->translateBy({0,-.05f,0});

			// Rotation
			if(rotLeft)
				model1->rotateBy({0,-5,0});
			if(rotRight)
				model1->rotateBy({0,5,0});
			if(rotUp)
				model1->rotateBy({5,0,0});
			if(rotDown)
				model1->rotateBy({-5,0,0});
		}
		else
		{
			// Movement
			if(moveLeft)
				GameEmGine::translateCameraBy({-.05f,0,0});
			if(moveRight)
				GameEmGine::translateCameraBy({.05f,0,0});
			if(moveForward)
				GameEmGine::translateCameraBy({0,0,.05f});
			if(moveBack)
				GameEmGine::translateCameraBy({0,0,-.05f});
			if(moveUp)
				GameEmGine::translateCameraBy({0,.05f,0});
			if(moveDown)
				GameEmGine::translateCameraBy({0,-.05f,0});

			// Rotation
			if(rotLeft)
				GameEmGine::RotateCameraBy(1,{0,-.5f,0});
			if(rotRight)
				GameEmGine::RotateCameraBy(1,{0,.5f,0});
			if(rotUp)
				GameEmGine::RotateCameraBy(1,{.5f,0,0});
			if(rotDown)
				GameEmGine::RotateCameraBy(1,{-.5f,0,0});
		}


		//	model1->print();
		//	puts("");
		//	//model2->print();
		//	printf(model1->collision3D(model2) ? "collision\n" : "no collision\n");
		//	puts("\n");

	}
private:

	Model* model1, * model2;
	Text testText;
};



int main()
{
	Test test;
	GameEmGine::init("Window Band (Previously Console Band) V3", 800, 400);
	GameEmGine::setScene(&test);
	GameEmGine::run();

	return 0;
}