#pragma once

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "physicsworld.h"

namespace engine {

	class PhysicsWorld;

	class KinematicCharacterController : public btKinematicCharacterController
	{
		friend class PhysicsWorld;


	};
}
