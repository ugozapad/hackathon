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
		assert(window && "Couldn't initialize graphics device without created window !");
		m_window = window;
	}

	void GraphicsDevice::shutdown()
	{
		m_window = nullptr;
	}

	void GraphicsDevice::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	GrTexture2D* GraphicsDevice::createTexture2D(TextureCreationDesc& desc)
	{
		return nullptr;
	}

}