#include "pch.h"
#include "game/playercomponent.h"

#include "engine/node.h"
#include "engine/inputmanager.h"
#include "engine/engine.h"
#include "engine/camera.h"
#include "game/gamestate.h"

namespace engine
{
	PlayerComponent::PlayerComponent()
	{
		auto world = Engine::ms_world->getPhysicsWorld()->getWorld();
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 10, 4));

		// #TODO: !!!
	/*	Node* nd = m_node;
		if (!nd)
			nd = GameState::getInstance()->getPlayerNode();

		glm::vec3 p = nd->getPosition();
		btVector3 bp = btVector3(p.x, p.y, p.x);
		startTransform.setOrigin(bp);*/

		btConvexShape* capsule = new btCapsuleShape(0.5, 1);
		
		m_ghostObject = new btPairCachingGhostObject();
		m_ghostObject->setWorldTransform(startTransform);
		world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		m_ghostObject->setCollisionShape(capsule);
		m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		charcontroller = new btKinematicCharacterController(m_ghostObject, capsule, 0.05f);
		charcontroller->setGravity(world->getGravity());

		world->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
		world->addAction(charcontroller);
		charcontroller->setMaxJumpHeight(1.5);
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
		updatePhysics(dt);
	}

	void PlayerComponent::updateCamera(float dt)
	{
		glm::vec3 pos = this->m_node->getPosition();
		InputManager* input = InputManager::getInstance();
		Camera* cam = CameraProxy::getInstance();
		float camSpeed = 10.0f * dt;
		glm::vec2 mousePos = input->getCursorPos();
		cam->m_position = pos;
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

		m_node->setPosition(pos);

		glm::vec3 rot = m_node->getRotation();
		rot.y = -yaw;
		m_node->setRotation(rot);
	}

	void PlayerComponent::updatePhysics(float dt)
	{
		btTransform t;
		t = charcontroller->getGhostObject()->getWorldTransform();
		InputManager* input = InputManager::getInstance();
		btVector3 pos = t.getOrigin();
		btQuaternion  quat = t.getRotation();
		float angle = quat.getAngle();
		btVector3 axis = quat.getAxis();
		m_node->setPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
		glm::quat q = glm::angleAxis(float(angle * (180.0 / 3.14)), glm::vec3(axis.getX(), axis.getY(), axis.getZ()));
		//getTransform()->SetRot(q);
		/*glm::vec3 fb = getTransform()->getForward();
		glm::vec3 rl = getTransform()->getRight();*/
		Camera* cam = CameraProxy::getInstance();
		glm::vec3 dir(0.0f);

		if (input->getKey(GLFW_KEY_W))
			dir += cam->m_direction;
		if (input->getKey(GLFW_KEY_S))
			dir -= cam->m_direction;

		if (input->getKey(GLFW_KEY_A))
			dir -= glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));
		if (input->getKey(GLFW_KEY_D))
			dir += glm::normalize(glm::cross(cam->m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

		if (input->getKey(GLFW_KEY_SPACE) && charcontroller->onGround())
			charcontroller->jump(btVector3(0.0f, 6.0f, 0.0f));

		if (dir != glm::vec3(0, 0, 0) && (input->getKey(GLFW_KEY_W) || input->getKey(GLFW_KEY_S) || input->getKey(GLFW_KEY_A) || input->getKey(GLFW_KEY_D)))
		{
			if (charcontroller->onGround())
				charcontroller->setWalkDirection(btVector3(dir.x, dir.y, dir.z).normalized() / 10);
			else
				charcontroller->setWalkDirection(btVector3(dir.x, dir.y, dir.z).normalized() / 30);
		}

		else
			charcontroller->setWalkDirection(btVector3(0, 0, 0));
	}

}