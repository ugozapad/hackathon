#include "pch.h"
#include "common/common.h"
#include "common/mallocallocator.h"

#include "core/context.h"
#include "core/timer.h"

#include "file/filedevice.h"
#include "file/archivemanager.h"

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
#include "graphics/drawablecomponent.h"
#include "graphics/staticmeshcomponent.h"
#include "graphics/imgui_mgr.h"

#include "audio/audiomanager.h"
#include "audio/audiosource.h"

#include "physics/physicscomponent.h"

#include "game/game.h"
#include "game/gamestate.h"
#include "game/playercomponent.h"
#include "game/skyboxcomponent.h"
#include "game/weaponcomponent.h"
//#include "game/cameralogic.h"
#include "game/mainmenu.h"

#include "common/freelistallocator.h"

#include "graphics/light.h"

#include "audio/musicmanager.h"

#include <GLFW/glfw3.h>

#include <io.h>

namespace engine
{
	GLFWwindow* g_engineWindow;
	View* g_engineView;

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

	void loadArchives()
	{
		ArchiveManager::getInstance()->loadArchive("data.pak");
	}

	void loadLevel()
	{
		if (MusicManager::getInstance()->isPlaying())
			MusicManager::getInstance()->stop();

		GameState::getInstance()->setGameState(GameState::GAME_STATE_RUNNING);

		// Get content manager ptr.
		ContentManager* contentManager = ContentManager::getInstance();

		// menu background stuff
		TextureMap* loadTex = contentManager->loadTexture("textures/load.bmp").get();

		Renderer::getInstance()->beginFrame();
		ScreenQuad::render(loadTex->getHWTexture());
		Renderer::getInstance()->endFrame();


		Engine::loadEmptyWorld();
		//	Engine::ms_world->getPhysicsWorld()->getWorld()->setGravity(btVector3(0.0, -1.0, 0.0));

			// static mesh to level
		auto levelNode = Engine::ms_world->createNode();
		auto levelMesh = levelNode->createComponentByType<StaticMeshComponent>();
		levelMesh->addModel(contentManager->loadModel("models/levels/l01_street.dae"));
		{
			glm::vec3 p = levelNode->getPosition();
			p.y = -1.0f;
			levelNode->setPosition(p);
		}

		//{
		//	for (int y = 0; y < 20; y++)
		//	{
		//		static float offsety = 0.0f;
		//		static float offset = 0.0f;
		//		for (int x = 0; x < 20; x++)
		//		{


		//			auto node = Engine::ms_world->createNodePtr();
		//			node->setPosition(glm::vec3(offset, 0.0f, offsety));
		//			auto nodeVisual = node->createComponentByType<GraphicsComponent>();
		//			nodeVisual->addModel(contentManager->loadModel("models/test1.dae"));

		//			offset += 5.0f;

		//		}

		//		offset = 0.0f;
		//		offsety += 5.0f;
		//	}

		//}

		// add test light
		auto testLevelLight = Engine::ms_world->createNode();
		testLevelLight->setPosition(glm::vec3(3, 2, 0.0f));
		auto testLevelLightComponent = testLevelLight->createComponentByType<LightComponent>();
		//	testLevelLightComponent->m_color = glm::vec3(1.0f, 0.0f, 0.0f);
		testLevelLightComponent->m_color = glm::vec3(0.3f);

		//{
		//	auto light = Engine::ms_world->createNode();
		//	light->setPosition(glm::vec3(20.0f, 5.0f, 1.0f));
		//	auto lightComponent = testLevelLight->createComponentByType<LightComponent>();
		//	lightComponent->m_color = glm::vec3(0.0f, 1.0f, 0.0f);
		//}

		//{
		//	auto light = Engine::ms_world->createNode();
		//	light->setPosition(glm::vec3(10.0f, 5.0f, 10.0f));
		//	auto lightComponent = testLevelLight->createComponentByType<LightComponent>();
		//	lightComponent->m_color = glm::vec3(0.0f, 0.0f, 1.0f);
		//}

		auto levelCollision = levelNode->createComponentByType<PhysicsComponent>();
		levelCollision->createShape(PhysicsBody::ShapeType::Box, levelNode->getPosition(), 0.0f, btVector3(30.f, 1.0f, 30.f));
		levelCollision->setStatic(true);

		// add skybox to world
		auto skyboxNode = Engine::ms_world->createNode();
		skyboxNode->createComponentByType<SkyboxComponent>();

		auto skyboxmesh = skyboxNode->createComponentByType<StaticMeshComponent>();
		skyboxmesh->addModel(contentManager->loadModel("models/skybox_1.dae"));

		// Add player
		auto playerNode = Engine::ms_world->createNode();
		playerNode->createComponentByType<PlayerComponent>();


		// weapon node (as child to player with little hack for weapon model)
		auto weaponNode = playerNode->createChild();

		// weapon component
		auto weapon = weaponNode->createComponentByType<WeaponComponent>();

		// add model
		auto weaponMesh = weaponNode->createComponentByType<StaticMeshComponent>();
		weaponMesh->addModel(contentManager->loadModel("models/test1.dae"));

		// little ugly but works
		weaponNode->setPosition(glm::vec3(0.4f, -0.2f, 0.4f));
		weaponNode->setScale(glm::vec3(0.15));

		//playerNode->createComponentByType<CameraLogicComponent>();

		//auto playerModel = playerNode->createComponentByType<GraphicsComponent>();
		//playerModel->addModel(contentManager->loadModel("models/test1.dae"));

		// add physics mesh
		//auto physicsTestStuffNode = Engine::ms_world->createNode();
		//physicsTestStuffNode->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

		//auto physicsTestStuffModel = physicsTestStuffNode->createComponentByType<StaticMeshComponent>();
		////physicsTestStuffModel->addModel(contentManager->loadModel("models/test1.dae"));

		//auto physComponent = physicsTestStuffNode->createComponentByType<PhysicsComponent>();
		//physComponent->createShape(PhysicsBody::ShapeType::Box, physicsTestStuffNode->getPosition(), 1.0f);
	}

	void glfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputManager::getInstance()->keyboardAction(key, action);
	}

	void glfwMouseCursorPosCallback(GLFWwindow* window, double x, double y)
	{
		InputManager::getInstance()->cursorPosAction((float)x, (float)y);
	}

	void initInputCallbacks(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, glfwKeyboardCallback);
		glfwSetCursorPosCallback(window, glfwMouseCursorPosCallback);
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
		std::string title = "Engine";
		g_engineWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (fullscreen)
			glfwSetWindowMonitor(g_engineWindow, glfwGetPrimaryMonitor(), 0, 0, width, height, g_graphicsOptions.m_refreshRate);

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
		// allocate fixed size memory for engine
		//size_t memory_size = 1024ULL * 1024 * 1024; //1GB
		//void* allocatedMemory = malloc(memory_size);

		// create and initialize system allocator
		//FreeListAllocator* freeListAllocator = new (allocatedMemory)FreeListAllocator(memory_size - sizeof(FreeListAllocator), pointer_help::add(allocatedMemory, sizeof(FreeListAllocator)));

		// set system allocator
		//g_sysAllocator = freeListAllocator;

		// set system allocator
		static MallocAllocator s_mallocAllocator;
		g_sysAllocator = &s_mallocAllocator;

		// initializing logger
		Logger::init();

		// Initialize file system
		FileDevice::instance()->setDefaultPath("data/");

		char buffer[256];
		GetCurrentDirectoryA(256, buffer);
		if (strstr(buffer, "bin"))
			FileDevice::instance()->setDefaultPath("../data/");

		spdlog::info("current path: {}", buffer);

		spdlog::info("working path: {}", FileDevice::instance()->getDefaultPath());

		std::string commandLine = createCommandLine(argc, argv);
		spdlog::info("command lile: {}", commandLine.c_str());

		if (commandLine.empty())
		{
			File* file = FileDevice::instance()->openFile("commandline.txt", FileAccess::Read);
			if (file->isValid())
			{
				file->readString(commandLine);
			}

			FileDevice::instance()->closeFile(file);
		}

		loadArchives();

		// init glfw
		glfwInit();

		spdlog::info("GLFW is initialized");

		// set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// initialize engine context
		std::shared_ptr<Context> content = std::make_shared<Context>();

		// initialize timer
		Timer::getInstance()->init();

		// create engine view
		createEngineView();
		glfwSetInputMode(g_engineWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
		Renderer* renderer = Renderer::getInstance();

		ImguiManager::getInstance()->init(g_engineWindow);

		// initialize audio manager
		AudioManager::getInstance()->init();

		// Get content manager ptr.
		ContentManager* contentManager = ContentManager::getInstance();

		// game init
		registerGameClasses();

		GameState* gameState = GameState::getInstance();
		gameState->setGameState(GameState::GAME_STATE_MAIN_MENU);

		//MusicManager::getInstance()->play("sounds/music/temp_01.mp3", true);

		// menu background stuff
		TextureMap* mainMenuTex = contentManager->loadTexture("textures/menu.bmp").get();

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

			if (gameState->getGameState() == GameState::GAME_STATE_RUNNING)
			{
				glfwSetInputMode(g_engineWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
			{
				glfwSetInputMode(g_engineWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			// update timer
			Timer::getInstance()->reset();

			ImguiManager::getInstance()->beginFrame();

			if (gameState->getGameState() == GameState::GAME_STATE_MAIN_MENU)
			{
				if (!MusicManager::getInstance()->isPlaying())
					MusicManager::getInstance()->play("sounds/music/menu.mp3", true);

				MainMenu::ms_instance.render();
			}
			else
			{
				// update camera
				CameraProxy::getInstance()->update();

				// run engine frame
				Engine::update();
			}

			// sound
			AudioManager::getInstance()->update();
			
			renderer->beginFrame();

			if (gameState->getGameState() == GameState::GAME_STATE_RUNNING)
				Renderer::getInstance()->renderView(g_engineView);

			if (gameState->getGameState() == GameState::GAME_STATE_MAIN_MENU)
				ScreenQuad::render(mainMenuTex->getHWTexture());

			ImguiManager::getInstance()->endFrame();

			renderer->endFrame();

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

		g_sysAllocator = nullptr;

		//freeListAllocator->~FreeListAllocator();
		//free(allocatedMemory);

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