#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/world.h"

namespace engine
{
	World* Engine::ms_world;

	void Engine::init()
	{
		spdlog::info("Initializing engine");

		//EngineTask worldUpdateTask(eastl::bind1st(&World::updateGraphicWorld, ms_world));
		TaskManager::getInstance()->createTaskWorkers();
	}

	void Engine::shutdown()
	{
		TaskManager::getInstance()->destroyTaskWorkers();
	}

}