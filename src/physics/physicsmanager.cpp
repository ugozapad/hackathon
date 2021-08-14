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

	PhysicsBody* PhysicsManager::createPhysicsBody(PhysicsBody::ShapeType shapeType, glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, btScalar mass, btVector3 size, btScalar radius)
	{
		// #TODO: HAVE NO SUCH TIME TO FIX MEMORY LEAK
		//return mem_new<PhysicsBody>(*PhysicsAllocator::ms_physicsAllocator, shapeType, position);
		return new PhysicsBody(shapeType, position, mass, size, radius);
	}

	void PhysicsManager::destroyPhysicsBody(PhysicsBody* physicsBody)
	{
		// #TODO: HAVE NO SUCH TIME TO FIX MEMORY LEAK
		//mem_delete(*PhysicsAllocator::ms_physicsAllocator, physicsBody);
		delete physicsBody;
	}
}

#include "physics/physicscomponent.h"

void engine::PhysicsManager::registerPhysicsClasses()
{
	engine::PhysicsComponent::registerObject();
}