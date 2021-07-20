#include "pch.h"
#include "engine/world.h"

#include "engine/components/logiccomponent.h"

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
			Node* node = *it;
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
		//for (EntityIt it = m_Entities.begin(); it != m_Entities.end(); ++it)
		//{
		//	Entity* entity = *it;
		//	VisualComponent* visualComponent = nullptr;

		//	if (entity)
		//		visualComponent = entity->GetComponentByType<VisualComponent>();
		//	if (visualComponent && visualComponent->IsActive())
		//		visualComponent->Draw();
		//}
	}

}