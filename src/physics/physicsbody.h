#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#include "btBulletDynamicsCommon.h"

namespace engine
{
	class PhysicsManager;
	class PhysicsWorld;

	/*!
	\brief Base class for any rigid/static body in physics world.
	*/
	class PhysicsBody
	{
		friend class PhysicsManager;
		friend class PhysicsWorld;
	public:
		/*!
		\brief Type of shape. If you using TriangleMesh, you need to pass collision mesh.
		*/
		enum class ShapeType
		{
			Box,
			Sphere,
			Cylinder,
			TriangleMesh,
		};

	public:
		PhysicsBody(PhysicsBody::ShapeType shapeType, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
		~PhysicsBody();

		glm::vec3 getPosition();

		btRigidBody* getBody() { return m_btRigidBody; }

	private:
		btRigidBody* m_btRigidBody;
	};
}

#endif // !PHYSICSBODY_H
