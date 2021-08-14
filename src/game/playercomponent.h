#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "engine/components/logiccomponent.h"

#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include"BulletCollision/CollisionDispatch/btGhostObject.h"

namespace engine
{
	class PlayerComponent : public LogicComponent
	{
		ImplementObject(PlayerComponent, LogicComponent);
	public:
		PlayerComponent();
		~PlayerComponent();

		static void registerObject();

		virtual void onNodeSet(Node* node);
		virtual void update(float dt);
		virtual void updateCamera(float dt);
		virtual void updatePhysics(float dt);
	private:
		btPairCachingGhostObject* m_ghostObject;
		btKinematicCharacterController* charcontroller;
	};
}

#endif // !PLAYERCOMPONENT_H
