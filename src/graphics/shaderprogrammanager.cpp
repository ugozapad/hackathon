#include "pch.h"
#include "graphics/shaderprogrammanager.h"

namespace engine
{
	ShaderProgramManager* ShaderProgramManager::instance()
	{
		static ShaderProgramManager s_instance;
		return &s_instance;
	}
}