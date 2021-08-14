#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "physicsworld.h"

namespace engine {

	class PhysicsWorld;


	class CharacterController 
	{
		friend class PhysicsWorld;
	public:
		CharacterController();
		~CharacterController();

		void createCharacter();

	private:
		btCapsuleShape* playerShape;
		btPairCachingGhostObject* ghostObj;
		btKinematicCharacterController* controller;
	};
}
