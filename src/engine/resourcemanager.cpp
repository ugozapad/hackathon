#include "pch.h"
#include "engine/resourcemanager.h"

#include "graphics/texturemap.h"

#include <future>

namespace engine
{
	ResourceManager g_resourceManager;

	TextureMap* loadTexture_impl(const std::string& name)
	{
		

		TextureMap* texture = new TextureMap;



		return nullptr;
	}


	TextureMap* ResourceManager::loadTexture(const std::string& name)
	{
		auto texture = std::find(m_textures.begin(), m_textures.end(), name);
		if (texture == m_textures.end())
		{
			auto handle = std::async(std::launch::async, loadTexture_impl, name);
			
		}

		return nullptr;
	}

	ModelBase* ResourceManager::loadMesh(const std::string& name)
	{
		return nullptr;
	}

	Material* ResourceManager::loadMaterial(const std::string& name)
	{
		return nullptr;
	}

}