#include "pch.h"

#include "graphics/gl/gltextures.h"
#include "graphics/gl/glframebuffer.h"

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

		glfwMakeContextCurrent(window);
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
		return (GrTexture2D*)mem_new<GLTexture2D>(*g_sysAllocator, desc);
	}

	void GraphicsDevice::deleteTexture2D(GrTexture2D* texture)
	{
		mem_delete(*g_sysAllocator, texture);
	}

	GrFramebuffer* GraphicsDevice::createFramebuffer(int width, int height)
	{
		return (GrFramebuffer*)mem_new<GLFramebuffer>(*g_sysAllocator, width, height);
	}

	void GraphicsDevice::deleteFramebuffer(GrFramebuffer* framebuffer)
	{
		mem_delete(*g_sysAllocator, framebuffer);
	}

}