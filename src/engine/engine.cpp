#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/world.h"

namespace engine
{
	World* Engine::ms_world;

	void Engine::init()
	{
		EngineTaskManager::getInstance()->createTaskWorkers();
	}

	void Engine::shutdown()
	{

	}

}