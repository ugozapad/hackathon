#include "pch.h"

#include "graphics/gl/glbuffers.h"
#include "graphics/gl/gltextures.h"
#include "graphics/gl/glframebuffer.h"

#include "graphics/graphicsdevice.h"

#include "gl/glext.h"

namespace engine
{
	// GL_ARB_debug_output
	PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;
	PFNGLDEBUGMESSAGEINSERTARBPROC glDebugMessageInsertARB;
	PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
	PFNGLGETDEBUGMESSAGELOGARBPROC glGetDebugMessageLogARB;

	void APIENTRY R_GLDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{

		/*if (type != GL_DEBUG_TYPE_ERROR)
			return;*/

		if (type == GL_DEBUG_TYPE_ERROR)
			spdlog::error("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
				type, severity, message);
		else
			spdlog::info("[gl]: {}type = 0x{}, severity = 0x{}, message = {}",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
				type, severity, message);

		//if (type == GL_DEBUG_TYPE_ERROR)
		//	DebugBreak();
	}

	void initGlDebug()
	{
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glfwGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glfwGetProcAddress("glGetDebugMessageLogARB");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(R_GLDebugOutput, 0);
	}

	static GLuint g_vao;
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
		
		initGlDebug();

		spdlog::info("OpenGL successful initialized");
		spdlog::info("OpenGL ver. {}", glGetString(GL_VERSION));
		spdlog::info("[device:{}] [vendor:{}]", glGetString(GL_RENDERER), glGetString(GL_VENDOR));

		glGenVertexArrays(1, &g_vao);
		glBindVertexArray(g_vao);
	}

	void GraphicsDevice::shutdown()
	{
		glDeleteVertexArrays(1, &g_vao);

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

	void GraphicsDevice::drawArray(PrimitiveMode mode, size_t startOf, size_t count)
	{
		GLenum primMode = 0;

		if (mode == PrimitiveMode::Triangles)
			primMode = GL_TRIANGLES;
		else if (mode == PrimitiveMode::TriangleStrip)
			primMode = GL_TRIANGLE_STRIP;

		glDrawArrays(primMode, startOf, count);
	}

	void GraphicsDevice::depthTest(bool value)
	{
		value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void GraphicsDevice::depthMask(bool value)
	{
		glDepthMask(value);
	}

	GrVertexBuffer* GraphicsDevice::createVertexBuffer(const BufferCreationDesc& desc)
	{
		return (GrVertexBuffer*)mem_new<GLVertexBuffer>(*g_sysAllocator, desc.m_data, desc.m_dataSize, desc.m_access);
	}

	GrVertexBuffer* GraphicsDevice::createVertexBuffer(void* data, size_t size, BufferAccess access)
	{
		return (GrVertexBuffer*)mem_new<GLVertexBuffer>(*g_sysAllocator, data, size, access);
	}

	void GraphicsDevice::deleteVertexBuffer(GrVertexBuffer* buffer)
	{
		mem_delete(*g_sysAllocator, buffer);
	}

	GrIndexBuffer* GraphicsDevice::createIndexBuffer(const BufferCreationDesc& desc)
	{
		return (GrIndexBuffer*)mem_new<GLIndexBuffer>(*g_sysAllocator, (unsigned int*)desc.m_data, desc.m_dataSize, desc.m_access);
	}

	GrIndexBuffer* GraphicsDevice::createIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
	{
		return (GrIndexBuffer*)mem_new<GLIndexBuffer>(*g_sysAllocator, data, size, access);
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

	void GraphicsDevice::setVertexBuffer(GrVertexBuffer* buffer)
	{
		if (buffer)
			buffer->bind();
		else
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GraphicsDevice::setIndexBuffer(GrIndexBuffer* buffer)
	{
		if (buffer)
			buffer->bind();
		else
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GraphicsDevice::setTexture2D(int slot, GrTexture2D* texture)
	{
		if (!texture) {
			spdlog::error("GraphicsDevice::setTexture2D: Texture is null pointer.");
			std::terminate();
		}

		if (slot > GL_TEXTURE31) {
			spdlog::error("GraphicsDevice::setTexture2D: Reach maxmimum value of slot position. {} (max {})", slot, 31);
			std::terminate();
		}

		glActiveTexture(GL_TEXTURE0 + slot);

		if (texture)
			glBindTexture(GL_TEXTURE_2D, texture->getHandle());
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GraphicsDevice::setFramebuffer(GrFramebuffer* framebuffer)
	{
		if (framebuffer)
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getHandle());
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}