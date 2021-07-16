#include "pch.h"
#include "graphics/graphicsdevice.h"

namespace engine
{

	GraphicsDevice* GraphicsDevice::instance()
	{
		static GraphicsDevice s_graphicsDevice;
		return &s_graphicsDevice;
	}

	void GraphicsDevice::init(GLFWwindow* window)
	{
		m_window = window;
	}

	void GraphicsDevice::shutdown()
	{

	}

	void GraphicsDevice::flushFrame()
	{
		glfwSwapBuffers(m_window);
	}

}