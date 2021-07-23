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

		// because no one want to initialize def_geom input, we need do it by own hand :(((
		{
			Shader* shader = createShader("def_geom");
			
			VertexDeclaration position;
			position.name = "position";
			position.size = 3;
			position.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(position);

			VertexDeclaration normal;
			normal.name = "normal";
			normal.size = 3;
			normal.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(normal);

			VertexDeclaration texcoord;
			texcoord.name = "texcoord";
			texcoord.size = 2;
			texcoord.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(texcoord);
		}
	}

	void ShaderManager::shutdown()
	{
		for (auto it : m_shaders)
			if ((it).second)
				mem_delete(*g_sysAllocator, it.second);

		m_shaders.clear();
	}

	Shader* ShaderManager::createShader(const std::string& name)
	{
		auto shaderAtTable = m_shaders.find(name);
		if (shaderAtTable == m_shaders.end())
		{
			std::string vsname;
			vsname += "data/shaders/";
			vsname += name;
			vsname += ".vsh";

			std::string fsname;
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