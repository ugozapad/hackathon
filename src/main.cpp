#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"

#include "core/context.h"
#include "core/timer.h"

#include "file/filedevice.h"

#include "engine/engine.h"
#include "engine/inputmanager.h"
#include "engine/camera.h"
#include "engine/node.h"
#include "engine/components/logiccomponent.h"
#include "engine/content/contentmanager.h"

#include "graphics/graphicsdevice.h"
#include "graphics/renderer.h"
#include "graphics/view.h"
#include "graphics/graphicsoptions.h"
#include "graphics/screenquad.h"
#include "graphics/shadermanager.h"
#include "graphics/graphicscomponent.h"

#include "audio/audiomanager.h"

#include "game/gamestate.h"
#include "game/birdcomponent.h"
#include "game/skyboxcomponent.h"

#include <GLFW/glfw3.h>

namespace engine
{
	static MallocAllocator g_allocator;
	static GLFWwindow* g_engineWindow;
	static View* g_engineView;

	std::string createCommandLine(int argc, char* argv[])
	{
		std::string commandline;

		for (int i = 1; i < argc; i++)
		{
			if (argv[i])
			{
				commandline.append(argv[i]);
				commandline.append(" ");
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
		std::string optionsFilename = "engine.ini";

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		//if (strstr(buffer, "bin"))
		//	optionsFilename = "engine.ini";
		//if (strstr(buffer, "build/src"))
		//	optionsFilename = "../../bin/engine.ini";

		if (!g_graphicsOptions.loadSettings(optionsFilename))
		{
			g_graphicsOptions.applyDefaultOptions();
			g_graphicsOptions.saveSettings(optionsFilename);
		}

		// create window
		int width = g_graphicsOptions.m_width, height = g_graphicsOptions.m_height;
		bool fullscreen = g_graphicsOptions.m_fullscreen;
		std::string title = "engine";
		g_engineWindow = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

		spdlog::info("Created window '{0}' [{1}x{2}] fullscreen:{3}", title.c_str(), width, height, fullscreen);

		g_engineView = mem_new<View>(*g_sysAllocator);

		int realWidth, realHeight;
		glfwGetWindowSize(g_engineWindow, &realWidth, &realHeight);

		g_engineView->m_width = realWidth;
		g_engineView->m_height = realHeight;
		g_engineView->m_fov = 75.0f;
		g_engineView->m_znear = 0.01f;
		g_engineView->m_zfar = 10000.0f;
		g_engineView->updateInternalValues();

		CameraProxy::getInstance()->setView(g_engineView);
	}

	int main(int argc, char* argv[])
	{
		// initializing logger
		Logger::init();

		// set system allocator
		g_sysAllocator = &g_allocator;

		// Initialize file system
		FileDevice::getInstance()->setDefaultPath("");

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		if (strstr(buffer, "bin"))
			FileDevice::getInstance()->setDefaultPath("../");

		spdlog::info("working path: {}", FileDevice::getInstance()->getDefaultPath());

		std::string commandLine = createCommandLine(argc, argv);
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
		std::shared_ptr<Context> content = std::make_shared<Context>();

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

		// initialize audio manager
		AudioManager::getInstance()->init();

		// Get content manager ptr.
		ContentManager* contentManager = ContentManager::getInstance();

		// hack: render loading screen
		{
			std::shared_ptr<TextureMap> loadTex = contentManager->loadTexture("data/textures/load.bmp");

			graphicsDevice->clear(ClearRenderTarget);
			ScreenQuad::render(loadTex->getHWTexture());
			graphicsDevice->swapBuffers();
		}

		// game init
		BirdComponent::registerObject();
		SkyboxComponent::registerObject();

		Engine::loadEmptyWorld();

		// add skybox to world
		std::shared_ptr<Node> skyboxNode = Engine::ms_world->createNodePtr();
		skyboxNode->createComponentByType<SkyboxComponent>();

		std::shared_ptr<GraphicsComponent> skyboxmesh = skyboxNode->createComponentByType<GraphicsComponent>();
		skyboxmesh->addModel(contentManager->loadModel("data/models/skybox_1.dae"));

		// Add bird
		std::shared_ptr<Node> birdNode = Engine::ms_world->createNodePtr();
		birdNode->createComponentByType<BirdComponent>();

		std::shared_ptr<GraphicsComponent> graphicsComponent = birdNode->createComponentByType<GraphicsComponent>();
		graphicsComponent->addModel(contentManager->loadModel("data/models/test1.dae"));

		// only for test, add second bird
		std::shared_ptr<Node> childBirdNode = birdNode->createChild();
		glm::vec3 testPos = childBirdNode->getPosition();
		testPos.x = 1.0f;
		testPos.z = -1.0f;
		childBirdNode->setPosition(testPos);

		// add test model
		std::shared_ptr<GraphicsComponent> childBirdMesh = childBirdNode->createComponentByType<GraphicsComponent>();
		childBirdMesh->addModel(contentManager->loadModel("data/models/test1.dae"));

		std::shared_ptr<TextureMap> logoTexture = ContentManager::getInstance()->loadTexture("data/textures/logo.bmp");

		GameState* gameState = GameState::getInstance();

		while (!glfwWindowShouldClose(g_engineWindow))
		{
			if (InputManager::getInstance()->getKey(GLFW_KEY_ESCAPE))
				break;

			if (InputManager::getInstance()->getKey(GLFW_KEY_F11))
				Renderer::getInstance()->makeScreenshot();

			if (InputManager::getInstance()->getKey(GLFW_KEY_F10))
				ShaderManager::getInstance()->reloadShaders();

			if (strstr(commandLine.c_str(), "-quit"))
				break;

			glfwPollEvents();

			// update timer
			Timer::getInstance()->reset();

			// update camera
			Camera* camera = CameraProxy::getInstance();

			glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			float radius = 5.0f;
			//float camX = sin(glfwGetTime()) * radius;
			//float camZ = cos(glfwGetTime()) * radius;
			float camX = radius;
			float camZ = radius;

			glm::vec3 pos = glm::vec3(camX, 0.0f, camZ);
			view = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			camera->m_position = pos;
			camera->getView()->m_view = view;

			// run engine frame
			Engine::update();

			graphicsDevice->clear(/*ClearRenderTarget |*/ ClearDepth);

			bool showIntro = false;
			if (showIntro)
				ScreenQuad::render(logoTexture->getHWTexture());
			else
				Renderer::getInstance()->renderView(g_engineView);

			graphicsDevice->swapBuffers();

			Timer::getInstance()->update();
		}

		spdlog::info("Exiting engine ...");

		AudioManager::getInstance()->shutdown();

		Renderer::getInstance()->shutdown();
		Renderer::destroyInstance();

		graphicsDevice->shutdown();

		Engine::shutdown();

		g_graphicsOptions.saveSettings("engine.ini");

		mem_delete(*g_sysAllocator, g_engineView);

		glfwTerminate();

		return 0;
	}
}

#ifdef WIN32
HANDLE g_semaphore;

int main(int argc, char* argv[])
{
	g_semaphore = CreateSemaphoreA(NULL, 1, 1, "engine_semaphore");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 1;

	return engine::main(argc, argv);
}
#endif // WIN32