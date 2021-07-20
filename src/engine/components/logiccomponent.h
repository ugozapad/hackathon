#ifndef LOGICCOMPONENT_H
#define LOGICCOMPONENT_H

#include "engine/components/component.h"

namespace engine
{
	// Logic component for node (run update function once at game frame)
	class LogicComponent : public Component
	{
		ImplementObject(LogicComponent, Component);
	public:
		static void registerCompoent();

	public:
		LogicComponent();
		virtual ~LogicComponent();

		// Update component once per frame.
		virtual void update(float delta);
	};
}

#endif // !LOGICCOMPONENT_H
