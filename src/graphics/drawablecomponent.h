#ifndef DRAWABLECOMPONENT_H
#define DRAWABLECOMPONENT_H

#include "engine/components/component.h"

namespace engine
{
	class DrawableComponent : public Component
	{
		ImplementObject(DrawableComponent, Component);

	public:
		DrawableComponent();
		~DrawableComponent();

		static void registerObject();
	};
}

#endif // !DRAWABLECOMPONENT_H
