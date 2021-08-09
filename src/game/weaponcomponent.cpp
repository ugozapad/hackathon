#include "pch.h"
#include "weaponcomponent.h"

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

	}
}