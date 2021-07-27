#ifndef CAMERALOGIC_H
#define CAMERALOGIC_H

#include "engine/components/logiccomponent.h"

namespace engine
{
	class CameraLogicComponent : public LogicComponent
	{
		ImplementObject(CameraLogicComponent, LogicComponent);
	public:
		static void registerObject();

		CameraLogicComponent();
		~CameraLogicComponent();

		virtual void update(float dt);
	};
}

#endif // !CAMERALOGIC_H
