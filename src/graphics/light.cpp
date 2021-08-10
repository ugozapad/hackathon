#include "pch.h"
#include "graphics/light.h"
#include "graphics/lightcontroller.h"

#include "engine/node.h"

namespace engine
{
	LightComponent::LightComponent()
	{
		m_color = glm::vec3(1.0);
		m_ambientColor = glm::vec3(0.2);
		m_specularColor = glm::vec3(1.0);
		m_shininess = 1.0f;

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

	void LightComponent::getRenderData(LightRenderData* data)
	{
		data->m_pos = m_node->getPosition();
		data->m_dir = glm::vec3(1.0);
		data->m_color = m_color;
		data->m_ambientColor = m_ambientColor;
		data->m_specularColor = m_specularColor;
		data->m_shininess = m_shininess;
	}

}