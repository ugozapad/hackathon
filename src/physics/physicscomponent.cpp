#include "pch.h"
#include "physics/physicscomponent.h"
#include "physics/physicsmanager.h"
#include "engine/engine.h"
#include "engine/world.h"
#include "physics/physicsworld.h"

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

	void PhysicsComponent::createShape(PhysicsBody::ShapeType shapeType, glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, btScalar mass)
	{
		ASSERT(!m_physicsBody && "Failed to create second shape!");

		m_physicsBody = PhysicsManager::getInstance()->createPhysicsBody(shapeType, position, mass);

		// #HACK: HACK HACK HACK
		Engine::ms_world->getPhysicsWorld()->addBodyToWorld(m_physicsBody);
	}

	void PhysicsComponent::onNodeSet(Node* node)
	{
		Component::onNodeSet(node);
	}

	void PhysicsComponent::setStatic(bool value)
	{
		m_isStatic = value;
		m_physicsBody->getBody()->setCollisionFlags(value ? btCollisionObject::CF_STATIC_OBJECT : btCollisionObject::CF_DYNAMIC_OBJECT);
	}

	void PhysicsComponent::update(float delta)
	{
		// hack 
		m_node->setPosition(getPhysicsBody()->getPosition());
	}

}
