#include "pch.h"
#include "game/game.h"

#include "game/birdcomponent.h"
#include "game/skyboxcomponent.h"
#include "game/cameralogic.h"

namespace engine
{
	void registerGameClasses()
	{
		BirdComponent::registerObject();
		SkyboxComponent::registerObject();
		CameraLogicComponent::registerObject();
	}
}
