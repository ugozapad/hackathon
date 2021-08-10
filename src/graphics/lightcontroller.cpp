#include "pch.h"
#include "graphics/lightcontroller.h"
#include "graphics/light.h"

namespace engine
{
	LightController LightController::ms_instance;

	void LightController::addLight(LightComponent* light)
	{
		ASSERT(light);
	}

	void LightController::removeLight(LightComponent* light)
	{
		ASSERT(light);
	}
}