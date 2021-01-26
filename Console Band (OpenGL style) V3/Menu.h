#pragma once
#include <GameEmGine.h>
#include <EmGineAudioPlayer.h>

class Menu:public Scene
{
public:
	void init()
	{
		setSkyBox("Skyboxes/skybox/");
		enableSkyBox(true);

		title.setText("Coolio");
		title.textSize(100);
		title.translate({0,0,0});
		//title.rotate({180,0,0});
		GameEmGine::addText(&title);

		OrthoPeramiters ortho{0,(float)Game::getWindowWidth(),(float)Game::getWindowHeight(),0,0,500};
		GameEmGine::setCameraType(Camera::ORTHOGRAPHIC, &ortho);
	}

	void update(double dt)
	{}

private:
	Text title;

};