#include "pch.h"
#include "engine/world.h"

#include "engine/components/logiccomponent.h"

#include "graphics/staticmeshcomponent.h"

#include "core/timer.h"

namespace engine
{
	void World::registerObject()
	{
		Context::getInstance()->registerObject<World>();
	}

	World::World()
	{
		m_physicsWorld = mem_new<PhysicsWorld>(*g_sysAllocator);
	}

	World::~World()
	{
		mem_delete(*g_sysAllocator, m_physicsWorld);
	}

	void World::release()
	{
		m_nodes.clear();
	}

	void World::updatePhysicsWorld()
	{
		float delta = Timer::getInstance()->getDelta();
		m_physicsWorld->step(1.0f / 24.0f);
	}

	void World::updateLogicWorld()
	{
		for (NodeIt it = m_nodes.begin(); it != m_nodes.end(); ++it)
		{
			Node* node = (*it).get();
			if (node)
			{
				for (Node::ComponentIt compIt = node->m_components.begin(); compIt != node->m_components.end(); ++compIt)
				{
					Component* component = compIt->get();
					if (component && component->isActive() && component->isA<LogicComponent>())
					{
						reinterpret_cast<LogicComponent*>(component)->update(Timer::getInstance()->getDelta());
					}
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
				StaticMeshComponent* graphicsComponent = node->getComponentByType<StaticMeshComponent>();
				if (graphicsComponent)
				{
					graphicsComponent->render();
				}
			}
		}
	}

	std::shared_ptr<Node> World::createNode()
	{
		std::shared_ptr<Node> node = Context::getInstance()->createObject<Node>();
		m_nodes.push_back(node);
		return node;
	}

	Node* World::createNode_NoSharedPtr()
	{
		return createNode().get();
	}

}