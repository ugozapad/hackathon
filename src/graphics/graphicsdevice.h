#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "graphics/vertexbuffer.h"
#include "graphics/indexbuffer.h"
#include "graphics/texture.h"
#include "graphics/framebuffer.h"

#include <GLFW/glfw3.h>

namespace engine
{
	enum ClearFlags
	{
		ClearRenderTarget = 1 << 0,
		ClearDepth = 1 << 0,
		ClearStencil = 1 << 0,
	};

	class GraphicsDevice : public Singleton<GraphicsDevice>
	{
	private:
		static GraphicsDevice ms_instance;

	public:
		static GraphicsDevice* instance();

	public:
		void init(GLFWwindow* window);
		void shutdown();

		// clear color
		void clearColor(float r, float g, float b, float a);

		// clear
		void clear(uint32_t flags);

		// swap buffers
		void swapBuffers();

		// drawing
		void drawArray(PrimitiveMode mode, size_t startOf, size_t count);

		// state
		void depthTest(bool value);
		void depthMask(bool value);

		// device creation
		GrVertexBuffer* createVertexBuffer(void* data, size_t size, BufferAccess access);
		GrIndexBuffer* createIndexBuffer(unsigned int* data, size_t size, BufferAccess access);

		GrVertexBuffer* createVertexBuffer(const BufferCreationDesc& desc);
		void deleteVertexBuffer(GrVertexBuffer* buffer);

		GrIndexBuffer* createIndexBuffer(const BufferCreationDesc& desc);
		void deleteIndexBuffer(GrIndexBuffer* buffer);

		GrTexture2D* createTexture2D(TextureCreationDesc& desc);
		void deleteTexture2D(GrTexture2D* texture);

		GrFramebuffer* createFramebuffer(int width, int height);
		void deleteFramebuffer(GrFramebuffer* framebuffer);

		// setters
		void setVertexBuffer(GrVertexBuffer* buffer);
		void setIndexBuffer(GrIndexBuffer* buffer);
		void setTexture2D(int slot, GrTexture2D* texture);
		void setFramebuffer(GrFramebuffer* framebuffer);

	private:
		GLFWwindow* m_window;
	};
}

#endif // !GRAPHICS_DEVICE_H
