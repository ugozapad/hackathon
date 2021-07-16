#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/world.h"

namespace engine
{
	World* Engine::ms_world;

	void Engine::init()
	{
		//EngineTask worldUpdateTask(eastl::bind1st(&World::updateGraphicWorld, ms_world));
		//EngineTaskManager::getInstance()->createTaskWorkers();
	}

	void Engine::shutdown()
	{

	}

}