#include "pch.h"
#include "engine/world.h"

#include "engine/components/logiccomponent.h"

#include "graphics/graphicscomponent.h"

#include "core/timer.h"

namespace engine
{
	void World::registerObject()
	{
		Context::getInstance()->registerObject<World>();
	}

	World::World()
	{
	}

	World::~World()
	{
	}

	void World::updatePhysicWorld()
	{

	}

	void World::updateLogicWorld()
	{
		for (NodeIt it = m_nodes.begin(); it != m_nodes.end(); ++it)
		{
			Node* node = (*it).get();
			if (node)
			{
				LogicComponent* logicComponent = node->getComponentByType<LogicComponent>();
				if (logicComponent)
				{
					logicComponent->update(Timer::getInstance()->getDelta());
				}
			}
		}
	}

	void World::updateGraphicWorld()
	{
		for (NodeIt it = m_nodes.begin(); it != m_nodes.end(); ++it)
		{
			Node* node = (*it).get();
			if (node)
			{
				GraphicsComponent* graphicsComponent = node->getComponentByType<GraphicsComponent>();
				if (graphicsComponent)
				{
					graphicsComponent->render();
				}
			}
		}
	}

	std::shared_ptr<Node> World::createNodePtr()
	{
		std::shared_ptr<Node> node = Context::getInstance()->createObject<Node>();
		m_nodes.push_back(node);
		return node;
	}

	Node* World::createNode()
	{
		return createNodePtr().get();
	}

}