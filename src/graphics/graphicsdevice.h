#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "graphics/vertexbuffer.h"
#include "graphics/indexbuffer.h"
#include "graphics/texture.h"
#include "graphics/framebuffer.h"

#include <GLFW/glfw3.h>

namespace engine
{
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

		// swap buffers
		void swapBuffers();

		// device creation
		GrVertexBuffer* createVertexBuffer(const BufferCreationDesc& desc);
		void deleteVertexBuffer(GrVertexBuffer* buffer);

		GrIndexBuffer* createIndexBuffer(const BufferCreationDesc& desc);
		void deleteIndexBuffer(GrIndexBuffer* buffer);

		GrTexture2D* createTexture2D(TextureCreationDesc& desc);
		void deleteTexture2D(GrTexture2D* texture);

		GrFramebuffer* createFramebuffer(int width, int height);
		void deleteFramebuffer(GrFramebuffer* framebuffer);

	private:
		GLFWwindow* m_window;
	};
}

#endif // !GRAPHICS_DEVICE_H
