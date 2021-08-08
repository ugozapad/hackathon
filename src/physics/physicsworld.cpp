#include "pch.h"
#include "physics/physicsworld.h"

namespace engine
{
	PhysicsWorld::PhysicsWorld()
	{
		m_collisionConfiguration = mem_new<btDefaultCollisionConfiguration>(*g_sysAllocator);
		m_dispatcher = mem_new<btCollisionDispatcher>(*g_sysAllocator, m_collisionConfiguration);
		m_overlappingPairCache = mem_new<btDbvtBroadphase>(*g_sysAllocator);
		m_solver = mem_new<btSequentialImpulseConstraintSolver>(*g_sysAllocator);
		m_body = mem_new<PhysicsBody>(*g_sysAllocator);
		m_world = mem_new<btDiscreteDynamicsWorld>(*g_sysAllocator, 
			m_dispatcher, 
			m_overlappingPairCache,
			m_solver, 
			m_collisionConfiguration,
			m_body);
		
	}

	PhysicsWorld::~PhysicsWorld()
	{
		// delete dynamics world
		mem_delete(*g_sysAllocator, m_world);

		// delete solver
		mem_delete(*g_sysAllocator, m_solver);

		// delete broadphase
		mem_delete(*g_sysAllocator, m_overlappingPairCache);

		// delete dispatcher
		mem_delete(*g_sysAllocator, m_dispatcher);

		// delete collision configuration
		mem_delete(*g_sysAllocator, m_collisionConfiguration);

		// delete physics body
		mem_delete(*g_sysAllocator, m_body);
	}

	void PhysicsWorld::step(float delta)
	{
		if (m_world)
			m_world->stepSimulation(delta);
	}
}
