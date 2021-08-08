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
		
		m_world = mem_new<btDiscreteDynamicsWorld>(*g_sysAllocator, 
			m_dispatcher, 
			m_overlappingPairCache,
			m_solver, 
			m_collisionConfiguration);
		
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

		
	}

	void PhysicsWorld::step(float delta)
	{
		if (m_world)
			m_world->stepSimulation(delta);
	}

	void PhysicsWorld::addRigidBody(PhysicsBody* body)
	{
		m_world->addRigidBody(body->m_btRigidBody);
	}
}
