#include "pch.h"
#include "graphics/light.h"
#include "graphics/lightcontroller.h"

namespace engine
{
	LightComponent::LightComponent()
	{
		LightController::getInstance()->addLight(this);
	}

	LightComponent::~LightComponent()
	{
		LightController::getInstance()->removeLight(this);
	}

	void LightComponent::registerObject()
	{
		Context::getInstance()->registerObject<LightComponent>();
	}

}