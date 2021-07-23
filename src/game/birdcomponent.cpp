#include "pch.h"
#include "game/birdcomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"

namespace engine
{
	BirdComponent::BirdComponent()
	{

	}

	BirdComponent::~BirdComponent()
	{

	}

	void BirdComponent::registerObject()
	{
		Context::getInstance()->registerObject<BirdComponent>();
	}

	void BirdComponent::update(float dt)
	{
		InputManager* input = InputManager::getInstance();
		glm::vec3 pos = m_node->getPosition();

		const float speed = 12.0f;
		const float fallingSpeed = 6.0f;
		pos.y -= fallingSpeed * dt;

		if (input->getKey(GLFW_KEY_SPACE))
			pos.y += speed * dt;

		m_node->setPosition(pos);
	}
}