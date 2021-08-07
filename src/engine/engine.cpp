#include "pch.h"
#include "engine/engine.h"
#include "engine/enginetask.h"
#include "engine/node.h"
#include "engine/world.h"

#include "engine/components/component.h"
#include "graphics/graphicscomponent.h"

#include "engine/content/contentmanager.h"

#include "physics/physicsmanager.h"

namespace engine
{
	World* Engine::ms_world;

	void registerEngineObjects()
	{
		Node::registerObject();
		Component::registerObject();
		World::registerObject();
	}

	void registerGraphicsObjects()
	{
		GraphicsComponent::registerObject();
	}

	void Engine::init()
	{
		spdlog::info("Initializing engine");

		registerEngineObjects();
		registerGraphicsObjects();

		ContentManager::getInstance()->init();

		TaskManager::getInstance()->createTaskWorkers();

		PhysicsManager::getInstance()->init();
	}

	void Engine::shutdown()
	{
		if (ms_world)
		{
			ms_world->release();
			mem_delete(*g_sysAllocator, ms_world);
		}
			

		PhysicsManager::getInstance()->shutdown();

		TaskManager::getInstance()->destroyTaskWorkers();

		ContentManager::getInstance()->shutdown();
	}

	void Engine::loadWorld(const std::string& filename)
	{

	}

	void Engine::loadEmptyWorld()
	{
		//ms_world = Context::getInstance()->createObject<World>();
		ms_world = mem_new<World>(*g_sysAllocator);
	}

	void Engine::update()
	{
		if (ms_world)
		{
			ms_world->updatePhysicsWorld();
			ms_world->updateLogicWorld();
		}
	}
}