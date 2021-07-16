#include "pch.h"
#include "engine/world.h"

namespace engine
{

World::World()
{
}

World::~World()
{
}

void World::updatePhysicWorld()
{
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