#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"
#include "file/filedevice.h"

#include "engine/engine.h"
#include "engine/content/contentmanager.h"

#include "graphics/graphicsdevice.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "graphics/graphicsoptions.h"

#include "graphics/screenquad.h"

#include <GLFW/glfw3.h>

namespace engine
{
	static MallocAllocator g_allocator;
	static GLFWwindow* g_engineWindow;
	static View* g_engineView;

	void createEngineView()
	{
		eastl::string optionsFilename = "hackathon.ini";

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		if (strstr(buffer, "bin"))
			optionsFilename = "hackathon.ini";
		if (strstr(buffer, "build/src"))
			optionsFilename = "../../bin/hackathon.ini";

		if (!g_graphicsOptions.loadSettings(optionsFilename))
		{
			g_graphicsOptions.applyDefaultOptions();
			g_graphicsOptions.saveSettings(optionsFilename);
		}

		// create window
		int width = g_graphicsOptions.m_width, height = g_graphicsOptions.m_height;
		bool fullscreen = g_graphicsOptions.m_fullscreen;
		eastl::string title = "Hackathon";
		g_engineWindow = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

		spdlog::info("Created window '{0}' [{1}x{2}] fullscreen:{3}", title.c_str(), width, height, fullscreen);
	}

	int main(int argc, char* argv[])
	{
		// initializing logger
		Logger::init();

		// set system allocator
		g_sysAllocator = &g_allocator;
	
		// Initialize file system
		FileDevice::getInstance()->setDefaultPath(".");

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		if (strstr(buffer, "bin"))
			FileDevice::getInstance()->setDefaultPath("../");
		
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

		// create engine view
		createEngineView();

		// initialize engine
		Engine::init();

		// initialize graphics device
		GraphicsDevice* graphicsDevice = GraphicsDevice::instance();
		graphicsDevice->init(g_engineWindow);

		// initialize renderer
		Renderer::createInstance();
		Renderer::getInstance()->init();

		eastl::shared_ptr<TextureMap> testTexture = ContentManager::getInstance()->loadTexture("data/textures/test.bmp");

		while (!glfwWindowShouldClose(g_engineWindow))
		{
			if (glfwGetKey(g_engineWindow, GLFW_KEY_ESCAPE))
				break;

			glfwPollEvents();
			
			graphicsDevice->clearColor(0.5f, 0.5f, 0.5f, 1.0f);
			graphicsDevice->clear(ClearRenderTarget | ClearDepth);

			ScreenQuad::render(testTexture->getHWTexture());

			graphicsDevice->swapBuffers();
		}

		spdlog::info("Exiting engine ...");

		Renderer::getInstance()->shutdown();
		Renderer::destroyInstance();

		Engine::shutdown();

		graphicsDevice->shutdown();

		return 0;
	}
}

int main(int argc, char* argv[])
{
	return engine::main(argc, argv);
}