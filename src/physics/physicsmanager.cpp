#include "pch.h"
#include "physics/physicsmanager.h"

#include "physics/physicsallocator.h"

namespace engine
{
	PhysicsManager PhysicsManager::ms_instance;

	void PhysicsManager::init()
	{
		// Initialize physics allocator
		PhysicsAllocator::init();
	}

	void PhysicsManager::shutdown()
	{
		// Shutdown physics allocator
		PhysicsAllocator::shutdown();
	}

}
