#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/node.h"
#include "engine/world.h"

#include "engine/components/component.h"

#include "engine/content/contentmanager.h"

namespace engine
{
	World* Engine::ms_world;

	void registerEngineObjects()
	{
		Node::registerObject();
		Component::registerObject();
		World::registerObject();
	}

	void Engine::init()
	{
		spdlog::info("Initializing engine");

		registerEngineObjects();

		ContentManager::getInstance()->init();

		TaskManager::getInstance()->createTaskWorkers();
	}

	void Engine::shutdown()
	{
		TaskManager::getInstance()->destroyTaskWorkers();

		ContentManager::getInstance()->shutdown();
	}

}