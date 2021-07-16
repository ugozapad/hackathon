#include "pch.h"

#include "graphics/gl/glbuffers.h"
#include "graphics/gl/gltextures.h"
#include "graphics/gl/glframebuffer.h"

#include "graphics/graphicsdevice.h"

namespace engine
{
	GraphicsDevice GraphicsDevice::ms_instance;

	GraphicsDevice* GraphicsDevice::instance()
	{
		return Singleton<GraphicsDevice>::getInstance();
	}

	void GraphicsDevice::init(GLFWwindow* window)
	{
		assert(window && "Couldn't initialize graphics device without created window !");
		m_window = window;

		glfwMakeContextCurrent(window);

		if (!gladLoadGL())
		{
			spdlog::error("!!! Failed to initialize OpenGL");
			std::terminate();
		}
		
		spdlog::info("OpenGL successful initialized");
		spdlog::info("OpenGL ver. {}", glGetString(GL_VERSION));
		spdlog::info("[Device:{}] [Vendor:{}]", glGetString(GL_RENDERER), glGetString(GL_VENDOR));
	}

	void GraphicsDevice::shutdown()
	{
		m_window = nullptr;
	}

	void GraphicsDevice::clearColor(float r, float g, float b, float a)
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	}

	void GraphicsDevice::clear(uint32_t flags)
	{
		if (flags > ClearStencil) {
			spdlog::error("GraphicsDevice::clear: Unknown flag passed");
			std::terminate();
		}

		GLenum clearFlags = 0;
		if (flags & ClearRenderTarget)
			clearFlags |= GL_COLOR_BUFFER_BIT;
		if (flags & ClearDepth)
			clearFlags |= GL_DEPTH_BUFFER_BIT;
		if (flags & ClearStencil)
			clearFlags |= GL_STENCIL_BUFFER_BIT;

		glClear(clearFlags);
	}

	void GraphicsDevice::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	GrVertexBuffer* GraphicsDevice::createVertexBuffer(const BufferCreationDesc& desc)
	{
		return (GrVertexBuffer*)mem_new<GLVertexBuffer>(*g_sysAllocator, desc.m_data, desc.m_dataSize, desc.m_access);
	}

	void GraphicsDevice::deleteVertexBuffer(GrVertexBuffer* buffer)
	{
		mem_delete(*g_sysAllocator, buffer);
	}

	GrIndexBuffer* GraphicsDevice::createIndexBuffer(const BufferCreationDesc& desc)
	{
		return (GrIndexBuffer*)mem_new<GLIndexBuffer>(*g_sysAllocator, (unsigned int*)desc.m_data, desc.m_dataSize, desc.m_access);
	}

	void GraphicsDevice::deleteIndexBuffer(GrIndexBuffer* buffer)
	{
		mem_delete(*g_sysAllocator, buffer);
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