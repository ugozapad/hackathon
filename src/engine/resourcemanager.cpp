#include "pch.h"
#include "engine/resourcemanager.h"

#include <future>

namespace engine
{
	ResourceManager g_resourceManager;

	std::shared_ptr<TextureMap> loadTextureMap_impl(const std::string& name)
	{
		return std::shared_ptr<TextureMap>();
	}

	std::shared_ptr<TextureMap> ResourceManager::loadTextureMap(const std::string& name)
	{
		auto handle = std::async(std::launch::async, loadTextureMap_impl, name);
		return handle.get();
	}

}