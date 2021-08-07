#include "pch.h"
#include "physics/physicscomponent.h"

namespace engine
{
	PhysicsComponent::PhysicsComponent()
	{

	}

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::registerObject()
	{
		Context::getInstance()->registerObject<PhysicsComponent>();
	}

}
