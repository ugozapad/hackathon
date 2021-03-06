#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "btBulletDynamicsCommon.h"

namespace engine
{
	class PhysicsBody;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void addBodyToWorld(PhysicsBody* body);

		void step(float delta);

		void addRigidBody(PhysicsBody* body);

		btDiscreteDynamicsWorld* getWorld() { return m_world; }

	private:
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_world;
	};
}

#endif // !PHYSICSWORLD_H
