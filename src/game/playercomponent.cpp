#include "pch.h"
#include "game/playercomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"
#include "physics/kinematiccharactercontroller.h"
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
		updateCamera(dt);
	}

	void PlayerComponent::updateCamera(float dt)
	{
		glm::vec3 pos = this->m_node->getPosition();
		InputManager* input = InputManager::getInstance();
		Camera* cam = CameraProxy::getInstance();
		float camSpeed = 10.0f * dt;
		glm::vec2 mousePos = input->getCursorPos();

		float xoffset = mousePos.x;
		float yoffset = mousePos.y;

		xoffset *= 0.1f;
		yoffset *= 0.1f;

		float yaw = -90.0f;
		float pitch = 0.0f;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		if (input->getKey(GLFW_KEY_W))
			pos += camSpeed * cam->m_direction;
		if (input->getKey(GLFW_KEY_S))
			pos -= camSpeed * cam->m_direction;

		if (input->getKey(GLFW_KEY_A))
			pos -= glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;
		if (input->getKey(GLFW_KEY_D))
			pos += glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * camSpeed;

		if (input->getKey(GLFW_KEY_Q))
			pos.y += camSpeed;
		if (input->getKey(GLFW_KEY_Z))
			pos.y -= camSpeed;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(-pitch));
		front.y = sin(glm::radians(-pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(-pitch));
		cam->m_direction = glm::normalize(front);

		cam->m_position = pos;
		m_node->setPosition(pos);
	}

}