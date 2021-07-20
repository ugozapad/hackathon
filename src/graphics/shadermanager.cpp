#include "pch.h"
#include "graphics/shadermanager.h"

#include "graphics/shader.h"

namespace engine
{
	ShaderManager ShaderManager::ms_instance;

	void ShaderManager::init()
	{
		// init engine default shaders
		createShader("quad");
		createShader("fade_in_out");
		createShader("def_geom");
	}

	void ShaderManager::shutdown()
	{

	}

	Shader* ShaderManager::createShader(const eastl::string& name)
	{
		auto shaderAtTable = m_shaders.find(name);
		if (shaderAtTable == m_shaders.end())
		{
			eastl::string vsname;
			vsname += "data/shaders/";
			vsname += name;
			vsname += ".vsh";

			eastl::string fsname;
			fsname += "data/shaders/";
			fsname += name;
			fsname += ".fsh";

			Shader* shader = mem_new<Shader>(*g_sysAllocator, name, vsname, fsname);
			m_shaders.emplace(name, shader);
			return shader;
		}

		return (*shaderAtTable).second;
	}
}