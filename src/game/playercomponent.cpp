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

		

		m_node->setPosition(pos);
	}

}