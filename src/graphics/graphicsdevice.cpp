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

	}

	void GraphicsDevice::shutdown()
	{

	}

}