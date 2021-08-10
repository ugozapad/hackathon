#ifndef LIGHT_H
#define LIGHT_H

#include "engine/components/component.h"
#include "lightcontroller.h"

namespace engine
{
	class LightComponent : public Component
	{
		ImplementObject(LightComponent, Component);
	public:
		LightComponent();
		~LightComponent();

		static void registerObject();


	};
}

#endif // !LIGHT_H
