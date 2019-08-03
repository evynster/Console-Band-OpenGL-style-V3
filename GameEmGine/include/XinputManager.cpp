#include "XinputManager.h"

XinputDevice* XinputManager::controllers[4];

void XinputManager::update()
{

	for(int m_index = 0; m_index < 4; m_index++)
	{
		//check if controller is null
		if(!controllers[m_index])
			controllers[m_index] = new XinputDevice;

		//check if each controller is still connected
		if(controllerConnected(m_index))
		{
			controllers[m_index]->type = getControllerType(m_index);
			controllers[m_index]->m_index = m_index;
			controllers[m_index]->update();
		}
	}
}

bool XinputManager::controllerConnected(int m_index)
{
	XINPUT_STATE connected;
	return XInputGetState(m_index, &connected) == ERROR_SUCCESS;
}

XinputDevice* XinputManager::getController(int m_index)
{
	return controllers[m_index];
}

CONTROLLER_TYPE XinputManager::getControllerType(int m_index)
{
	static XINPUT_CAPABILITIES info;
	XInputGetCapabilities(m_index, NULL, &info);
	
	switch(info.SubType)
	{
	case XINPUT_DEVSUBTYPE_GAMEPAD:
		return XINPUT_CONTROLLER;
	case XINPUT_DEVSUBTYPE_GUITAR:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_GUITAR_BASS:
		return XINPUT_GUITAR;
	case XINPUT_DEVSUBTYPE_DRUM_KIT:
		return XINPUT_DRUM;
	}

	return XINPUT_UNKNOWN;
}
