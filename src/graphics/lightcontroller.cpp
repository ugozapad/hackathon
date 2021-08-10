#include "pch.h"
#include "graphics/lightcontroller.h"
#include "graphics/light.h"

namespace engine
{
	LightController LightController::ms_instance;

	void LightController::addLight(LightComponent* light)
	{
		ASSERT(light);
		m_lights.push_back(light);
	}

	void LightController::removeLight(LightComponent* light)
	{
		ASSERT(light);
		
		typedef std::vector<LightComponent*>::iterator LT;

		LT I = m_lights.begin();
		LT E = m_lights.end();

		for (; I != E;)
			if (*I == light)
				break;

		m_lights.erase(I);
	}
}