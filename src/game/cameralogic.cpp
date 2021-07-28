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
		float radius = 5.0f;
		float camX = radius;
		float camZ = radius;
		glm::vec3 pos = glm::vec3(camX, 0.0f, camZ);
		camera->m_position = pos;
		camera->m_direction = node->getPosition();
		camera->update();
	}

}