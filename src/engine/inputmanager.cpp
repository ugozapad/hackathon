#include "pch.h"
#include "engine/inputmanager.h"

namespace engine
{
	InputManager InputManager::ms_inputManager;

	void InputManager::keyboardAction(uint32_t keyId, bool state)
	{
		if (keyId >= 420)
		{
			spdlog::error("InputManager::keyboardAction: keyId >= 420");
			std::terminate();
		}

		m_keys[keyId] = state;
	}
}