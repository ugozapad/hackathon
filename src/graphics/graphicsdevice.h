#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include <GLFW/glfw3.h>

#include "graphics/texture.h"

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

	private:
		GLFWwindow* m_window;
	};
}

#endif // !GRAPHICS_DEVICE_H
