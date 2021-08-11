#include "pch.h"
#include "graphics/lightmanager.h"
#include "graphics/light.h"

namespace engine
{
	LightManager LightManager::ms_instance;

	void LightManager::addLight(LightComponent* light)
	{
		ASSERT(light);
		m_lights.push_back(light);
	}

	void LightManager::removeLight(LightComponent* light)
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