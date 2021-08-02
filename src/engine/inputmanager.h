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
		// ACTIONS
		void keyboardAction(uint32_t keyId, bool state);
		void cursorPosAction(float x, float y);

		// GETTERS
		bool getKey(uint32_t keyid);
		glm::vec2 getCursorPos() { return m_cursorPos; }

	private:
		bool m_keys[420];
		glm::vec2 m_cursorPos;
	};
}

#endif // !INPUTMANAGER_H
