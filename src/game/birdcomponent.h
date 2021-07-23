#ifndef BIRDCOMPONENT_H
#define BIRDCOMPONENT_H

#include "engine/components/logiccomponent.h"

namespace engine
{
	class BirdComponent : public LogicComponent
	{
		ImplementObject(BirdComponent, LogicComponent);
	public:
		BirdComponent();
		~BirdComponent();

		static void registerObject();

		virtual void update(float dt);
	};
}

#endif // !BIRDCOMPONENT_H
