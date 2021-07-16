#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

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
	};
}

#endif // !GRAPHICS_DEVICE_H
