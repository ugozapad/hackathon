#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "engine/components/component.h"

namespace engine
{
	class PhysicsComponent : public Component
	{
		ImplementObject(PhysicsComponent, Component);
	public:
		PhysicsComponent();
		virtual ~PhysicsComponent();

		static void registerObject();

	};
}

#endif // !PHYSICSCOMPONENT_H
