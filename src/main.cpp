#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"
#include "file/filedevice.h"

#include "engine/engine.h"

#include "graphics/graphicsdevice.h"
#include "graphics/renderer.h"

#include <GLFW/glfw3.h>

namespace engine
{
	static MallocAllocator g_allocator;

	int main(int argc, char* argv[])
	{
		// initializing logger
		Logger::init();

		// set system allocator
		g_sysAllocator = &g_allocator;
	
		// Initialize file system
		FileDevice::getInstance()->setDefaultPath(".");

		// Load command-line
		eastl::string commandLine;

		File* file = FileDevice::getInstance()->openFile("commandline.txt", FileAccess::Read);
		if (file->isValid())
		{
			file->seek(FileSeek::End, 0);
			size_t fileLength = file->tell();
			file->seek(FileSeek::Begin, 0);

			commandLine.resize(fileLength);
			file->read(&commandLine[0], fileLength);

			commandLine.push_back('\0');
		}

		FileDevice::getInstance()->closeFile(file);

		// init glfw
		glfwInit();

		spdlog::info("GLFW is initialized!");

		// set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

		// create window
		int width = 800, height = 600;
		bool fullscreen = false;
		eastl::string title = "Hackathon";
		GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

		spdlog::info("Created window '{0}' [{1}x{2}] fullscreen:{3}", title.c_str(), width, height, fullscreen);

		// initialize engine
		Engine::init();

		// initialize graphics device
		GraphicsDevice* graphicsDevice = GraphicsDevice::instance();
		graphicsDevice->init(window);

		while (!glfwWindowShouldClose(window))
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
				break;

			glfwPollEvents();
			
			graphicsDevice->swapBuffers();
		}

		spdlog::info("Exiting engine ...");

		Engine::shutdown();

		graphicsDevice->shutdown();

		return 0;
	}
}

int main(int argc, char* argv[])
{
	return engine::main(argc, argv);
}