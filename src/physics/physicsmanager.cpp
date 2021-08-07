#include "pch.h"
#include "physics/physicsmanager.h"

#include "physics/physicsallocator.h"

namespace engine
{
	PhysicsManager PhysicsManager::ms_instance;

	void PhysicsManager::init()
	{
		registerPhysicsClasses();

		// Initialize physics allocator
		PhysicsAllocator::init();
	}

	void PhysicsManager::shutdown()
	{
		// Shutdown physics allocator
		PhysicsAllocator::shutdown();
	}
}

#include "physics/physicscomponent.h"

void engine::PhysicsManager::registerPhysicsClasses()
{
	engine::PhysicsComponent::registerObject();
}