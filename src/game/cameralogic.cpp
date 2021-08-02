#include "pch.h"
#include "game/cameralogic.h"
#include "game/gamestate.h"
#include "engine/node.h"
#include "engine/camera.h"
#include "engine/inputmanager.h"

namespace engine
{
	void CameraLogicComponent::registerObject()
	{
		Context::getInstance()->registerObject<CameraLogicComponent>();
	}

	CameraLogicComponent::CameraLogicComponent()
	{

	}

	CameraLogicComponent::~CameraLogicComponent()
	{

	}

	void CameraLogicComponent::update(float dt)
	{
		InputManager* input = InputManager::getInstance();
		Camera* camera = CameraProxy::getInstance();

		// update camera
		float radius = 0.0f;
		float camX = radius;
		float camZ = radius;
		glm::vec3 pos = glm::vec3(camX, 0.0f, camZ);
		camera->m_position = pos;

		glm::vec3 dir = camera->m_direction;

		if (input->getKey(GLFW_KEY_W))
			dir.z -= 10.0f * dt;
		if (input->getKey(GLFW_KEY_S))
			dir.z += 10.0f * dt;

		if (input->getKey(GLFW_KEY_A))
			dir.x -= 10.0f * dt;
		if (input->getKey(GLFW_KEY_D))
			dir.x += 10.0f * dt;

		if (input->getKey(GLFW_KEY_Q))
			dir.y += 10.0f * dt;
		if (input->getKey(GLFW_KEY_Z))
			dir.y -= 10.0f * dt;

		camera->m_direction = dir;
		camera->update();
	}

}