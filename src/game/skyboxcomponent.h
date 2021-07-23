#ifndef SKYBOXCOMPONENT_H
#define SKYBOXCOMPONENT_H

#include "engine/components/logiccomponent.h"

namespace engine
{
	class SkyboxComponent : public LogicComponent
	{
		ImplementObject(SkyboxComponent, LogicComponent);
	public:
		static void registerObject();

	public:
		SkyboxComponent();
		~SkyboxComponent();

		virtual void update(float dt);
	};
}

#endif // !SKYBOXCOMPONENT_H
