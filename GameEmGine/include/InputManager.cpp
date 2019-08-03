#include "InputManager.h"

#pragma region Static Variables
//std::function<void(int)>
//InputManager::m_controllerConneced,
//InputManager::m_controllerDisconnected;
std::function<void(int, int)>
InputManager::m_keyUp,
InputManager::m_keyInitDown,
InputManager::m_keyHeldDown,
InputManager::m_mouseButtonPress,
InputManager::m_mouseButtonRelease;
//InputManager::m_controllerConnection;
std::function<void(int, int, int)>
InputManager::m_keyAll,
InputManager::m_mouseButtonAll;
#pragma endregion

InputManager::InputManager()
{
	glfwInit();
	glfwSetKeyCallback(glfwGetCurrentContext(), keyUpdate);
	glfwSetMouseButtonCallback(glfwGetCurrentContext(), mouseButtonUpdate);
	//glfwSetJoystickCallback(xinputConnectionUpdate);
}

InputManager::~InputManager()
{}

void InputManager::mouseButtonPressCallback(std::function<void(int, int)>mouseButton)
{
	m_mouseButtonPress = mouseButton;
}

void InputManager::mouseButtonReleaseCallback(std::function<void(int, int)>mouseButton)
{
	m_mouseButtonRelease = mouseButton;
}

void InputManager::mouseButtonUpdate(GLFWwindow *, int button, int state, int mods)
{
	if(m_mouseButtonAll != nullptr)
		m_mouseButtonAll(state, button, mods);

	if(state == MOUSE_PRESSED)
		if(m_mouseButtonPress != nullptr)
			m_mouseButtonPress(button, mods);

	if(state == MOUSE_RELEASED)
		if(m_mouseButtonRelease != nullptr)
			m_mouseButtonRelease(button, mods);
}

void InputManager::keyUpdate(GLFWwindow *, int key, int scancode, int state, int mods)
{
	scancode;
	if(m_keyAll != nullptr)
		m_keyAll(state, key, mods);

	if(state == KEY_PRESSED)  //Key has been pressed initially
		if(m_keyInitDown != nullptr)
			m_keyInitDown(key, mods);

	if(state == KEY_HELD)
		if(m_keyHeldDown != nullptr)
			m_keyHeldDown(key, mods);

	if(state == KEY_RELEASED) //more of a stroke since release is only called if a key is pressed then released
		if(m_keyUp != nullptr)
			m_keyUp(key, mods);
}

//void InputManager::xinputConnectionUpdate(int controller, int connected)
//{
//	controllerUpdate();
//	if(m_controllerConnection != nullptr)
//		m_controllerConnection(controller, connected);
//
//	if(m_controllerConneced != nullptr)
//		if(connected == GLFW_CONNECTED)
//			m_controllerConneced(controller);
//
//	if(m_controllerDisconnected != nullptr)
//		if(connected == GLFW_DISCONNECTED)
//			m_controllerDisconnected(controller);
//}

void InputManager::setMouseButtonAllCallback(std::function<void(int, int, int)>mouseButton)
{
	m_mouseButtonAll = mouseButton;
}

Coord2D<> InputManager::getMouseCursorPosition()
{
	double x, y;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return {(float)x,(float)y};
}

void InputManager::setKeyPressedCallback(std::function<void(int, int)>key)
{
	m_keyInitDown = key;
}

void InputManager::setKeyReleasedCallback(std::function<void(int, int)>key)
{
	m_keyUp = key;
}

void InputManager::setKeyAllCallback(std::function<void(int, int, int)>key)
{
	m_keyAll = key;
}

//void InputManager::controllerConnectedCallback(std::function<void(int)>controllerConnection)
//{
//	m_controllerConneced = controllerConnection;
//}
//
//void InputManager::controllerDisconnectedCallback(std::function<void(int)>controllerConnection)
//{
//	m_controllerDisconnected = controllerConnection;
//}
//
//void InputManager::controllerAllConnectionCallback(std::function<void(int, int)>connected)
//{
//	m_controllerConnection = connected;
//}

int InputManager::controllersConnected()
{
	int count = 0;
	for(int a = 0; a < 4; a++)
		count += XinputManager::controllerConnected(a);
	return count;
}

bool InputManager::isControllerConnected(unsigned int m_index)
{
	return XinputManager::controllerConnected(m_index);
}

XinputDevice& InputManager::getController(unsigned int m_index)
{
	return *XinputManager::getController(m_index);
}

void InputManager::controllerUpdate()
{
	XinputManager::update();
}