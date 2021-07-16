#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"
#include "file/filedevice.h"
#include "graphics/graphicsdevice.h"
#include "graphics/renderer.h"

#include <GLFW/glfw3.h>

namespace engine
{
	static MallocAllocator g_allocator;

	int main(int argc, char* argv[])
	{
		g_sysAllocator = &g_allocator;

		FileDevice::instance()->setDefaultPath(".");

		// init glfw
		glfwInit();

		// create window
		int width = 800, height = 600;
		bool fullscreen = false;
		GLFWwindow* window = glfwCreateWindow(width, height, "Engine", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	
		// initialize graphics device
		GraphicsDevice::instance()->init(window);



		return 0;
	}
}

int main(int argc, char* argv[])
{
	return engine::main(argc, argv);
}