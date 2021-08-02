#include "pch.h"
#include "engine/inputmanager.h"

namespace engine
{
	InputManager InputManager::ms_inputManager;

	void InputManager::keyboardAction(uint32_t keyId, bool state)
	{
		m_keys[keyId] = state;
	}

	void InputManager::cursorPosAction(float x, float y)
	{
		m_cursorPos.x = x;
		m_cursorPos.y = y;
	}

	bool InputManager::getKey(uint32_t keyid)
	{
		return m_keys[keyid];
	}

}