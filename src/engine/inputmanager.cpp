#include "pch.h"
#include "engine/inputmanager.h"

namespace engine
{
	InputManager InputManager::ms_inputManager;

	void InputManager::keyboardAction(uint32_t keyId, bool state)
	{
		m_keys[keyId] = state;
	}

	bool InputManager::getKey(uint32_t keyid)
	{
		return m_keys[keyid];
	}

}