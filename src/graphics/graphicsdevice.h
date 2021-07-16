#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "graphics/texture.h"
#include "graphics/framebuffer.h"

#include <GLFW/glfw3.h>

namespace engine
{
	class GraphicsDevice
	{
	public:
		static GraphicsDevice* instance();
	public:
		void init(GLFWwindow* window);
		void shutdown();

		// swap buffers
		void swapBuffers();

		// device creation
		GrTexture2D* createTexture2D(TextureCreationDesc& desc);
		void deleteTexture2D(GrTexture2D* texture);

		GrFramebuffer* createFramebuffer(int width, int height);
		void deleteFramebuffer(GrFramebuffer* framebuffer);

	private:
		GLFWwindow* m_window;
	};
}

#endif // !GRAPHICS_DEVICE_H
