#pragma once
#include "XinputManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Utilities.h"

/***Enums***/

enum  KeyState
{
	KEY_RELEASED,
	KEY_PRESSED,
	KEY_HELD
};

enum ModKey
{
	KEY_MOD_SHIFT = 1,
	KEY_MOD_CONTROL = 2,
	KEY_MOD_ALT = 4,
	KEY_MOD_SUPER = 8
};

enum MouseState
{
	MOUSE_RELEASED,
	MOUSE_PRESSED
};

enum MouseButton
{
	MOUSE_BUTTON_1,
	MOUSE_BUTTON_2,
	MOUSE_BUTTON_3,
	MOUSE_BUTTON_4,
	MOUSE_BUTTON_5,
	MOUSE_BUTTON_6,
	MOUSE_BUTTON_7,
	MOUSE_BUTTON_8,

	LEFT_BUTTON = 0,
	RIGHT_BUTTON = 1,
	MIDDLE_BUTTON = 2,
	LAST_BUTTON = 7
};

///***Structs***/


/***Class***/

class InputManager
{
public:

	//must be called to work
	static void init();

	static void mouseButtonPressCallback(std::function<void(int, int)> mouseButton);

	static void mouseButtonReleaseCallback(std::function<void(int, int)> mouseButton);

	static Coord2D<> getMouseCursorPosition();

	/*
	Callback for whenever any key is pressed
	*/
	static void setKeyPressedCallback(std::function<void(int, int)> key);

	/*
	Callback for whenever any key is released
	*/
	static void setKeyReleasedCallback(std::function<void(int, int)> key);

	/*
	Callback for any key pressed, held, or released
	*/
	static void setKeyAllCallback(std::function<void(int, int, int)> key);

	static int controllersConnected();

	static bool isControllerConnected(unsigned int m_index);

	static XinputDevice* getController(unsigned int m_index);

	static void controllerUpdate();

private:
	
	static void mouseButtonUpdate(GLFWwindow *, int button, int action, int mods);
	static void keyUpdate(GLFWwindow *, int key, int scancode, int action, int mods);
	//static void xinputConnectionUpdate(int controller, int connected);

	static void setMouseButtonAllCallback(std::function<void(int, int, int)> mouseButton);
	
	//static std::function<void(int)>
	//	m_controllerConneced,
	//	m_controllerDisconnected;
	static std::function<void(int, int)>
		m_keyUp,
		m_keyInitDown,
		m_keyHeldDown,
		m_mouseButtonPress,
		m_mouseButtonRelease;
	//	m_controllerConnection;
	static std::function<void(int, int, int)>
		m_keyAll,
		m_mouseButtonAll;
};

