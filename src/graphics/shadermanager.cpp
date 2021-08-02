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
		createShader("forw_geom");

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

		{
			Shader* shader = createShader("forw_geom");

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

		{
			Shader* shader = createShader("def_light");

			VertexDeclaration position;
			position.name = "position";
			position.size = 2;
			position.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(position);

			VertexDeclaration texcoord;
			texcoord.name = "texcoord";
			texcoord.size = 2;
			texcoord.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(texcoord);
		}

		{
			Shader* shader = createShader("pp_test");

			VertexDeclaration position;
			position.name = "position";
			position.size = 2;
			position.type = VertexDeclaration::Float;
			shader->addVertexDeclaration(position);

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
			vsname += "shaders/";
			vsname += name;
			vsname += ".vsh";

			std::string fsname;

			fsname += "shaders/";
			fsname += name;
			fsname += ".fsh";

			Shader* shader = mem_new<Shader>(*g_sysAllocator, name, vsname, fsname);
			m_shaders.emplace(name, shader);
			return shader;
		}

		return (*shaderAtTable).second;
	}

	void ShaderManager::reloadShaders()
	{
		for (auto it : m_shaders)
			it.second->hotReload();
	}

}