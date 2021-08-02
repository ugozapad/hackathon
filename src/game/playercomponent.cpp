#include "pch.h"
#include "game/playercomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"

#include "game/gamestate.h"

namespace engine
{
	PlayerComponent::PlayerComponent()
	{

	}

	PlayerComponent::~PlayerComponent()
	{

	}

	void PlayerComponent::registerObject()
	{
		Context::getInstance()->registerObject<PlayerComponent>();
	}

	void PlayerComponent::onNodeSet(Node* node)
	{
		Component::onNodeSet(node);
		GameState::getInstance()->setPlayerNode(node);
	}

	void PlayerComponent::update(float dt)
	{
		InputManager* input = InputManager::getInstance();
		glm::vec3 pos = m_node->getPosition();

		//if (input->getKey(GLFW_KEY_W))
		//	pos.z -= 10.0f * dt;
		//if (input->getKey(GLFW_KEY_S))
		//	pos.z += 10.0f * dt;

		//if (input->getKey(GLFW_KEY_A))
		//	pos.x -= 10.0f * dt;
		//if (input->getKey(GLFW_KEY_D))
		//	pos.x += 10.0f * dt;

		//if (input->getKey(GLFW_KEY_Q))
		//	pos.y += 10.0f * dt;
		//if (input->getKey(GLFW_KEY_Z))
		//	pos.y -= 10.0f * dt;

		m_node->setPosition(pos);
	}

}