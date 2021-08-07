#include "pch.h"
#include "physics/physicsbody.h"

namespace engine
{
	PhysicsBody::PhysicsBody(PhysicsBody::ShapeType shapeType, glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/)
	{
		btCollisionShape* shape = nullptr;

		switch (shapeType)
		{
		case ShapeType::Box:
			shape = new btBoxShape(btVector3(1.0, 1.0, 1.0));
			break;
		//case ShapeType::Sphere:
		//	break;
		//case ShapeType::Cylinder:
		//	break;
		//case ShapeType::TriangleMesh:
		//	break;

		default:
			Core::error("PhysicsBody::PhysicsBody: unknowed shape type");
		}

		btVector3 localInertia(0, 0, 0);
		float mass = 1.0f;
		shape->calculateLocalInertia(mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo info(btScalar(0.3), nullptr, shape, localInertia);
		info.m_friction = btScalar(0.5);
		info.m_rollingFriction = btScalar(0.3);
		info.m_restitution = 0.5f;

		m_btRigidBody = new btRigidBody(info);
		m_btRigidBody->forceActivationState(DISABLE_DEACTIVATION);

	}

	PhysicsBody::~PhysicsBody()
	{

	}
}
