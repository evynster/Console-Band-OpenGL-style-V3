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

	InputManager();
	~InputManager();

	void mouseButtonPressCallback(std::function<void(int, int)> mouseButton);

	void mouseButtonReleaseCallback(std::function<void(int, int)> mouseButton);

	void mouseButtonPressCallback(void mouseButton(int button, int mods));

	void mouseButtonReleaseCallback(void mouseButton(int button, int mods));

	void setMouseButtonAllCallback(void mouseButton(int state, int button, int mods));

	static Coord2D<> getMouseCursorPosition();

	void setKeyPressedCallback(std::function<void(int, int)> key);

	void setKeyReleasedCallback(std::function<void(int, int)> key);

	void setKeyAllCallback(std::function<void(int, int, int)> key);

	void controllerConnectedCallback(std::function<void(int)> controllerConnection);

	void controllerDisconnectedCallback(std::function<void(int)> controllerConnection);

	void controllerAllConnectionCallback(std::function<void(int, int)> connected);

	/*
	Callback for whenever any key is pressed or held
	*/
	void setKeyPressedCallback(void key(int key, int mod));

	/*
	Callback for whenever any key is released
	*/
	void setKeyReleasedCallback(void key(int key, int mod));

	/*
	Callback for any key pressed or released
	*/
	void setKeyAllCallback(void key(int state, int key, int mod));
	
	//void controllerConnectedCallback(void controllerConnection(int controllerNum));
	//
	//void controllerDisconnectedCallback(void controllerConnection(int controllerNum));
	//
	//void controllerAllConnectionCallback(void controllerConnection(int controllerNum, int connected));

	int controllersConnected();

	bool isControllerConnected(unsigned int m_index);

	XinputDevice& getController(unsigned int m_index);

	static void controllerUpdate();

	//template<class T> friend void operator=(T[], T*);

private:
	
	static void mouseButtonUpdate(GLFWwindow *, int button, int action, int mods);
	static void keyUpdate(GLFWwindow *, int key, int scancode, int action, int mods);
	//static void xinputConnectionUpdate(int controller, int connected);

	void setMouseButtonAllCallback(std::function<void(int, int, int)> mouseButton);
	
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

//template<class T>
// void operator=(T a[], T* b)
//{
//	int size = sizeof(a) / sizeof(T);
//	for(int c = 0; c < size; c++)
//		a[c] = b[c];
//}
