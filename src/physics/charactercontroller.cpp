#include "pch.h"
#include "charactercontroller.h"

#include "engine/engine.h"

namespace engine
{
	CharacterController::CharacterController()
	{
		playerShape = nullptr;
		ghostObj = nullptr;
		controller = nullptr;
	}

	CharacterController::~CharacterController()
	{
		delete playerShape;
		delete ghostObj;
		delete controller;
	}

	void CharacterController::createCharacter()
	{
		playerShape = new btCapsuleShape(0.25, 1);
		ghostObj = new btPairCachingGhostObject();
		ghostObj->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 20, 0)));
		Engine::ms_world->getPhysicsWorld()->getWorld()->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		ghostObj->setCollisionShape(playerShape);
		ghostObj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		controller = new btKinematicCharacterController(ghostObj, playerShape, 0.5f);
		Engine::ms_world->getPhysicsWorld()->getWorld()->addCollisionObject(ghostObj, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
		Engine::ms_world->getPhysicsWorld()->getWorld()->addAction(controller);
	}
}