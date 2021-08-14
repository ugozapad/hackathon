#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "physics/physicsbody.h"

namespace engine
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		static PhysicsManager ms_instance;

	public:
		void init();
		void shutdown();

		PhysicsBody* createPhysicsBody(PhysicsBody::ShapeType shapeType, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), btScalar mass = 0.0f, btVector3 size = btVector3(1.0f, 1.0f, 1.0f), btScalar radius = 1.0f);
		void destroyPhysicsBody(PhysicsBody* physicsBody);

	private:
		void registerPhysicsClasses();
	};
}

#endif // !PHYSICSMANAGER_H
