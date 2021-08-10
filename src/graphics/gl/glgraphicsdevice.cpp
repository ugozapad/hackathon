#include "pch.h"

#include "graphics/gl/glad/glad.h"
#include "gl/glext.h"

#include "graphics/gl/glgraphicsdevice.h"
#include "graphics/gl/glbuffers.h"
#include "graphics/gl/gltextures.h"
#include "graphics/gl/glframebuffer.h"

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

		if (type != GL_DEBUG_TYPE_ERROR)
			return;

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

	engine::GLGraphicsDevice GLGraphicsDevice::ms_instance;

	void GLGraphicsDevice::init(GLFWwindow* window)
	{
		assert(window && "Couldn't initialize graphics device without created window !");
		m_window = window;

		glfwMakeContextCurrent(window);

		if (!gladLoadGL())
		{
			Core::error("!!! Failed to initialize OpenGL");
		}

		initGlDebug();

		spdlog::info("OpenGL successful initialized");
		spdlog::info("OpenGL ver. {}", glGetString(GL_VERSION));
		spdlog::info("[device:{}] [vendor:{}]", glGetString(GL_RENDERER), glGetString(GL_VENDOR));

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);

		glGenVertexArrays(1, &g_vao);
		glBindVertexArray(g_vao);
	}

	void GLGraphicsDevice::shutdown()
	{
		glDeleteVertexArrays(1, &g_vao);

		m_window = nullptr;
	}

	void GLGraphicsDevice::clearColor(float r, float g, float b, float a)
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	}

	void GLGraphicsDevice::clear(uint32_t flags)
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

	void GLGraphicsDevice::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void GLGraphicsDevice::drawArray(PrimitiveMode mode, size_t startOf, size_t count)
	{
		GLenum primMode = 0;

		if (mode == PrimitiveMode::Triangles)
			primMode = GL_TRIANGLES;
		else if (mode == PrimitiveMode::TriangleStrip)
			primMode = GL_TRIANGLE_STRIP;

		glDrawArrays(primMode, startOf, count);
	}

	void GLGraphicsDevice::drawElements(PrimitiveMode mode, size_t count)
	{
		GLenum primMode = 0;

		if (mode == PrimitiveMode::Triangles)
			primMode = GL_TRIANGLES;
		else if (mode == PrimitiveMode::TriangleStrip)
			primMode = GL_TRIANGLE_STRIP;

		//glDrawElements(primMode, count, GL_UNSIGNED_BYTE, NULL);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}

	void GLGraphicsDevice::depthTest(bool value)
	{
		value ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void GLGraphicsDevice::depthMask(bool value)
	{
		glDepthMask(value);
	}

	GrVertexBuffer* GLGraphicsDevice::createVertexBuffer(const BufferCreationDesc& desc)
	{
		return (GrVertexBuffer*)mem_new<GLVertexBuffer>(*g_sysAllocator, desc.m_data, desc.m_dataSize, desc.m_access);
	}

	GrVertexBuffer* GLGraphicsDevice::createVertexBuffer(void* data, size_t size, BufferAccess access)
	{
		return (GrVertexBuffer*)mem_new<GLVertexBuffer>(*g_sysAllocator, data, size, access);
	}

	void GLGraphicsDevice::deleteVertexBuffer(GrVertexBuffer* buffer)
	{
		mem_delete(*g_sysAllocator, buffer);
	}

	GrIndexBuffer* GLGraphicsDevice::createIndexBuffer(const BufferCreationDesc& desc)
	{
		return (GrIndexBuffer*)mem_new<GLIndexBuffer>(*g_sysAllocator, (unsigned int*)desc.m_data, desc.m_dataSize, desc.m_access);
	}

	GrIndexBuffer* GLGraphicsDevice::createIndexBuffer(unsigned int* data, size_t size, BufferAccess access)
	{
		return (GrIndexBuffer*)mem_new<GLIndexBuffer>(*g_sysAllocator, data, size, access);
	}

	void GLGraphicsDevice::deleteIndexBuffer(GrIndexBuffer* buffer)
	{
		mem_delete(*g_sysAllocator, buffer);
	}

	GrTexture2D* GLGraphicsDevice::createTexture2D(TextureCreationDesc& desc)
	{
		return (GrTexture2D*)mem_new<GLTexture2D>(*g_sysAllocator, desc);
	}

	void GLGraphicsDevice::deleteTexture2D(GrTexture2D* texture)
	{
		mem_delete(*g_sysAllocator, texture);
	}

	GrFramebuffer* GLGraphicsDevice::createFramebuffer(int width, int height)
	{
		return (GrFramebuffer*)mem_new<GLFramebuffer>(*g_sysAllocator, width, height);
	}

	void GLGraphicsDevice::deleteFramebuffer(GrFramebuffer* framebuffer)
	{
		mem_delete(*g_sysAllocator, framebuffer);
	}

	void GLGraphicsDevice::setVertexBuffer(GrVertexBuffer* buffer)
	{
		if (buffer)
			buffer->bind();
		else
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLGraphicsDevice::setIndexBuffer(GrIndexBuffer* buffer)
	{
		if (buffer)
			buffer->bind();
		else
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLGraphicsDevice::setTexture2D(int slot, GrTexture2D* texture)
	{
		//if (!texture) {
		//	Core::error("GraphicsDevice::setTexture2D: Texture is null pointer.");
		//}

		if (slot > GL_TEXTURE31) {
			Core::error("GraphicsDevice::setTexture2D: Reach maxmimum value of slot position. %i (max %i)", slot, 31);
		}

		glActiveTexture(GL_TEXTURE0 + slot);

		if (texture)
			glBindTexture(GL_TEXTURE_2D, texture->getHandle());
		else
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLGraphicsDevice::setFramebuffer(GrFramebuffer* framebuffer)
	{
		if (framebuffer)
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getHandle());
		else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLGraphicsDevice::setViewport(Viewport* viewport)
	{
		if (!viewport)
		{
			Core::error("GLGraphicsDevice::setViewport: viewport is null pointer");
		}

		glViewport(viewport->m_x, viewport->m_y, viewport->m_width, viewport->m_height);
	}
}