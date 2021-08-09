#include "pch.h"
#include "weaponcomponent.h"

#include "engine/node.h"

namespace engine
{
	WeaponComponent::WeaponComponent()
	{

	}

	WeaponComponent::~WeaponComponent()
	{

	}

	void WeaponComponent::registerObject()
	{
		Context::getInstance()->registerObject<WeaponComponent>();
	}

	void WeaponComponent::update(float dt)
	{
		glm::vec3 pos = m_node->getPosition();
		m_node->setPosition(pos);
	}

	void WeaponComponent::onNodeSet(Node* node)
	{
		Component::onNodeSet(node);
	}
}