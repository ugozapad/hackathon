#ifndef GLGRAPHICSDEVICE_H
#define GLGRAPHICSDEVICE_H

#include "graphics/graphicsdevice.h"

namespace engine
{
	class GLGraphicsDevice : public GraphicsDevice
	{
	private:
		static GLGraphicsDevice ms_instance;
	public:
		virtual void init(GLFWwindow* window);
		virtual void shutdown();

		// clear color
		virtual void clearColor(float r, float g, float b, float a);

		// clear
		virtual void clear(uint32_t flags);

		// swap buffers
		virtual void swapBuffers();

		// drawing
		virtual void drawArray(PrimitiveMode mode, size_t startOf, size_t count);

		// state
		virtual void depthTest(bool value);
		virtual void depthMask(bool value);

		// device creation
		virtual GrVertexBuffer* createVertexBuffer(void* data, size_t size, BufferAccess access);
		virtual GrIndexBuffer* createIndexBuffer(unsigned int* data, size_t size, BufferAccess access);

		virtual GrVertexBuffer* createVertexBuffer(const BufferCreationDesc& desc);
		virtual void deleteVertexBuffer(GrVertexBuffer* buffer);

		virtual GrIndexBuffer* createIndexBuffer(const BufferCreationDesc& desc);
		virtual void deleteIndexBuffer(GrIndexBuffer* buffer);

		virtual GrTexture2D* createTexture2D(TextureCreationDesc& desc);
		virtual void deleteTexture2D(GrTexture2D* texture);

		virtual GrFramebuffer* createFramebuffer(int width, int height);
		virtual void deleteFramebuffer(GrFramebuffer* framebuffer);

		// setters
		virtual void setVertexBuffer(GrVertexBuffer* buffer);
		virtual void setIndexBuffer(GrIndexBuffer* buffer);
		virtual void setTexture2D(int slot, GrTexture2D* texture);
		virtual void setFramebuffer(GrFramebuffer* framebuffer);

		// viewport
		virtual void setViewport(Viewport* viewport);

	};
}

#endif // !GLGRAPHICSDEVICE_H
