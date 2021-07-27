#include "pch.h"
#include "game/cameralogic.h"
#include "game/gamestate.h"
#include "engine/node.h"
#include "engine/camera.h"

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
		Camera* camera = CameraProxy::getInstance();
		Node* node = GameState::getInstance()->getPlayerNode();

		// update camera
		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		float radius = 5.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		float camX = radius;
		float camZ = radius;

		// look at on bird
		//float camX = node->getPosition().x;
		//float camZ = node->getPosition().z;

		glm::vec3 pos = glm::vec3(camX, 0.0f, camZ);
		view = glm::lookAt(pos, node->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
		camera->m_position = pos;
		camera->getView()->m_view = view;
	}

}