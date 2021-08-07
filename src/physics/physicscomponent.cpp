#include "pch.h"
#include "physics/physicscomponent.h"

#include "physics/physicsmanager.h"

namespace engine
{
	PhysicsComponent::PhysicsComponent()
	{
		m_physicsBody = nullptr;
	}

	PhysicsComponent::~PhysicsComponent()
	{
		if (m_physicsBody)
		{
			PhysicsManager::getInstance()->destroyPhysicsBody(m_physicsBody);
			m_physicsBody = nullptr;
		}
	}

	void PhysicsComponent::registerObject()
	{
		Context::getInstance()->registerObject<PhysicsComponent>();
	}

	void PhysicsComponent::createShape(PhysicsBody::ShapeType shapeType, glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/)
	{
		m_physicsBody = PhysicsManager::getInstance()->createPhysicsBody(shapeType, position);
	}
}
