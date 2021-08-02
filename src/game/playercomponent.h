#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "engine/components/logiccomponent.h"

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
	};
}

#endif // !PLAYERCOMPONENT_H
