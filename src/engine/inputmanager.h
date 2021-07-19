#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>

namespace engine
{
	class InputManager : public Singleton<InputManager>
	{
	private:
		static InputManager ms_inputManager;

	public:
		void keyboardAction(uint32_t keyId, bool state);

		bool getKey(uint32_t keyid);

	private:
		bool m_keys[420];
	};
}

#endif // !INPUTMANAGER_H
