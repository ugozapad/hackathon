#include "pch.h"
#include "game/skyboxcomponent.h"

#include "engine/node.h"
#include "engine/camera.h"

namespace engine
{
	void SkyboxComponent::registerObject()
	{
		Context::getInstance()->registerObject<SkyboxComponent>();
	}

	SkyboxComponent::SkyboxComponent()
	{

	}

	SkyboxComponent::~SkyboxComponent()
	{

	}

	void SkyboxComponent::update(float dt)
	{
		m_node->setPosition(CameraProxy::getInstance()->getPosition());
	}
}