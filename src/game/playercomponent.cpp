#include "pch.h"
#include "game/playercomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"

#include "engine/camera.h"
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
		Camera* cam = CameraProxy::getInstance();
		InputManager* input = InputManager::getInstance();
		glm::vec3 pos = m_node->getPosition();
		float camSpeed = 10.0f * dt;
		
		if (input->getKey(GLFW_KEY_W))
			pos += camSpeed * cam->m_direction;
		if (input->getKey(GLFW_KEY_S))
			pos -= camSpeed * cam->m_direction;

		if (input->getKey(GLFW_KEY_A))
			pos -= glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
		if (input->getKey(GLFW_KEY_D))
			pos += glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;

		if (input->getKey(GLFW_KEY_Q))
			pos.y += 10.0f * dt;
		if (input->getKey(GLFW_KEY_Z))
			pos.y -= 10.0f * dt;

		cam->m_position = pos;
		m_node->setPosition(pos);
		
	}

}