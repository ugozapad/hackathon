#include "pch.h"
#include "physics/physicsbody.h"

namespace engine
{
	PhysicsBody::PhysicsBody(PhysicsBody::ShapeType shapeType, glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, btScalar mass)
	{
		btCollisionShape* shape = nullptr;

		switch (shapeType)
		{
		case ShapeType::Box:
			shape = new btBoxShape(btVector3(1.0, 1.0, 1.0));
			break;
		case ShapeType::Sphere:
			shape = new btSphereShape(1.0f);
			break;
		case ShapeType::Cylinder:
			shape = new btCylinderShape(btVector3(1.0, 1.0, 1.0));
			break;
			//case ShapeType::TriangleMesh:
			//	break;

		default:
			Core::error("PhysicsBody::PhysicsBody: unknowed shape type");
		}

		btVector3 localInertia(0, 0, 0);

		if (!m_isStatic)
		{
			
			shape->calculateLocalInertia(mass, localInertia);
		}

		
		
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(position.x, position.y, position.z));

		btDefaultMotionState* motionState = new btDefaultMotionState(t);

		btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);

		
		
		m_btRigidBody = new btRigidBody(info);
		
		
		//m_btRigidBody->forceActivationState(DISABLE_DEACTIVATION);

	}

	PhysicsBody::~PhysicsBody()
	{

	}

	glm::vec3 PhysicsBody::getPosition()
	{
		btTransform t = m_btRigidBody->getWorldTransform();
		btVector3 p = t.getOrigin();
		return glm::vec3(p.x(), p.y(), p.z());
	}

	
}
