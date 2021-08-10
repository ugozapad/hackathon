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
		TextureMap* loadTexture(const std::string& name);
		ModelBase* loadMesh(const std::string& name);
		Material* loadMaterial(const std::string& name);

	private:
		std::unordered_map<std::string, TextureMap*> m_textures;
		std::unordered_map<std::string, ModelBase*> m_models;
		std::unordered_map<std::string, Material*> m_materials;
	};

	extern ResourceManager g_resourceManager;
}

#endif // !RESOURCEMANAGER_H
