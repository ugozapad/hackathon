#include "pch.h"
#include "game/birdcomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"

#include "game/gamestate.h"

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

	void BirdComponent::onNodeSet(Node* node)
	{
		Component::onNodeSet(node);
		GameState::getInstance()->setPlayerNode(node);
	}

	void BirdComponent::update(float dt)
	{
		InputManager* input = InputManager::getInstance();
		glm::vec3 pos = m_node->getPosition();

		const float speed = 12.0f;
		const float fallingSpeed = 6.0f;
		//pos.y -= fallingSpeed * dt;

		if (input->getKey(GLFW_KEY_SPACE))
			pos.y += speed * dt;
		if (input->getKey(GLFW_KEY_LEFT_CONTROL))
			pos.y -= speed * dt;

		if (input->getKey(GLFW_KEY_W))
			pos.x += speed * dt;
		if (input->getKey(GLFW_KEY_S))
			pos.x -= speed * dt;
		if (input->getKey(GLFW_KEY_A))
			pos.z += speed * dt;
		if (input->getKey(GLFW_KEY_D))
			pos.z -= speed * dt;

		m_node->setPosition(pos);
	}

}