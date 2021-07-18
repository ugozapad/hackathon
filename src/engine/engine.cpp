#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/world.h"

#include "engine/content/contentmanager.h"

namespace engine
{
	World* Engine::ms_world;

	void Engine::init()
	{
		spdlog::info("Initializing engine");

		ContentManager::getInstance()->init();

		//EngineTask worldUpdateTask(eastl::bind1st(&World::updateGraphicWorld, ms_world));
		TaskManager::getInstance()->createTaskWorkers();

	}

	void Engine::shutdown()
	{
		TaskManager::getInstance()->destroyTaskWorkers();

		ContentManager::getInstance()->shutdown();
	}

}