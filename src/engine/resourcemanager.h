#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "graphics/texturemap.h"
#include "graphics/model.h"
#include "graphics/material.h"

namespace engine
{
	class ResourceManager
	{
	public:
		std::shared_ptr<TextureMap> loadTextureMap(const std::string& name);
		std::shared_ptr<ModelBase> loadMesh(const std::string& name);
		std::shared_ptr<Material> loadMaterial(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<TextureMap>> m_textures;
	};

	extern ResourceManager g_resourceManager;
}

#endif // !RESOURCEMANAGER_H
