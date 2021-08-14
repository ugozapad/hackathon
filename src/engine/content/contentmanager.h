#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "engine/content/content.h"

#include "graphics/texturemap.h"
#include "graphics/model.h"
#include "graphics/material.h"

namespace engine
{
	class ContentThread;

	class ContentManager : public Singleton<ContentManager>
	{
		friend class ContentThread;
	private:
		static ContentManager ms_instance;

	public:
		void init();
		void shutdown();

		std::shared_ptr<TextureMap> loadTexture(const std::string& textureName);
		std::shared_ptr<ModelBase> loadModel(const std::string& modelName);
		std::shared_ptr<Material> loadMaterial(const std::string& materialName);

	private:
		std::unordered_map<std::string, std::shared_ptr<Content>> m_content;

		std::vector<std::pair<std::string, std::shared_ptr<Content>>> m_contentForLoad;

	};
}

#endif // !CONTENTMANAGER_H
