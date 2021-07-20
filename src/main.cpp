#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"
#include "file/filedevice.h"

#include "core/context.h"
#include "core/timer.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/camera.h"
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

	eastl::string createCommandLine(int argc, char* argv[])
	{
		eastl::string commandline;

		for (int i = 1; i < argc; i++)
		{
			if (argv[i])
			{
				commandline.append(argv[i]);
				//commandline.append(" ");
			}
		}

		return commandline;
	}

	void glfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputManager::getInstance()->keyboardAction(key, action);
	}

	void initInputCallbacks(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, glfwKeyboardCallback);
	}

	void createEngineView()
	{
		eastl::string optionsFilename = "hackathon.ini";

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		//if (strstr(buffer, "bin"))
		//	optionsFilename = "hackathon.ini";
		//if (strstr(buffer, "build/src"))
		//	optionsFilename = "../../bin/hackathon.ini";

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

		g_engineView = mem_new<View>(*g_sysAllocator);
		g_engineView->m_width = width;
		g_engineView->m_height = height;
		g_engineView->m_fov = 75.0f;
		g_engineView->m_znear = 0.01f;
		g_engineView->m_zfar = 10000.0f;
		g_engineView->updateInternalValues();

		CameraProxy::getInstance()->setCamera(mem_new<Camera>(*g_sysAllocator));
		CameraProxy::getInstance()->setView(g_engineView);
	}

	int main(int argc, char* argv[])
	{
		// initializing logger
		Logger::init();

		// set system allocator
		g_sysAllocator = &g_allocator;
	
		// Initialize file system
		FileDevice::getInstance()->setDefaultPath("./");

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		if (strstr(buffer, "bin"))
			FileDevice::getInstance()->setDefaultPath("../");
		
		eastl::string commandLine = createCommandLine(argc, argv);
		spdlog::info("command lile: {}", commandLine.c_str());

		if (commandLine.empty())
		{
			File* file = FileDevice::getInstance()->openFile("commandline.txt", FileAccess::Read);
			if (file->isValid())
			{
				file->readString(commandLine);
			}

			FileDevice::getInstance()->closeFile(file);
		}

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

		// initialize engine context
		eastl::shared_ptr<Context> content = eastl::make_shared<Context>();

		// initialize timer
		Timer::getInstance()->init();

		// create engine view
		createEngineView();

		// initialize input callbacks
		initInputCallbacks(g_engineWindow);

		// initialize engine
		Engine::init();

		// initialize graphics device
		GraphicsDevice* graphicsDevice = GraphicsDevice::instance();
		graphicsDevice->init(g_engineWindow);

		// initialize renderer
		Renderer::createInstance();
		Renderer::getInstance()->init(g_engineView);

		eastl::shared_ptr<TextureMap> logoTexture = ContentManager::getInstance()->loadTexture("data/textures/logo.bmp");
 
		while (!glfwWindowShouldClose(g_engineWindow))
		{
			if (InputManager::getInstance()->getKey(GLFW_KEY_ESCAPE))
				break;

			if (commandLine == "-quit")
				break;

			glfwPollEvents();

			// update timer
			Timer::getInstance()->update();
			
			graphicsDevice->clearColor(0.5f, 0.5f, 0.5f, 1.0f);
			graphicsDevice->clear(ClearRenderTarget | ClearDepth);

			bool showIntro = true;
			if (showIntro)
				ScreenQuad::render(logoTexture->getHWTexture());
			else
				Renderer::getInstance()->renderView(g_engineView);

			graphicsDevice->swapBuffers();
		}

		spdlog::info("Exiting engine ...");

		Renderer::getInstance()->shutdown();
		Renderer::destroyInstance();

		Engine::shutdown();

		graphicsDevice->shutdown();
		
		mem_delete(*g_sysAllocator, g_engineView);

		glfwTerminate();

		return 0;
	}
}

#ifdef WIN32
HANDLE g_semaphore;

int main(int argc, char* argv[])
{
	g_semaphore = CreateSemaphoreA(NULL, 1, 1, "hackathon_semaphore");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 1;

	return engine::main(argc, argv);
}
#endif // WIN32