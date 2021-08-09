#include "pch.h"
#include "game/game.h"

#include "game/playercomponent.h"
#include "game/skyboxcomponent.h"
#include "game/weaponcomponent.h"
//#include "game/cameralogic.h"

namespace engine
{
	void registerGameClasses()
	{
		PlayerComponent::registerObject();
		SkyboxComponent::registerObject();
		//CameraLogicComponent::registerObject();
		WeaponComponent::registerObject();
	}
}
