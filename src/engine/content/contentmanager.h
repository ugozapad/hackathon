#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "engine/content/content.h"

#include "graphics/texturemap.h"
#include "graphics/model.h"

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

		eastl::shared_ptr<TextureMap> loadTexture(const eastl::string& textureName);
		eastl::shared_ptr<ModelBase> loadModel(const eastl::string& modelName);

	private:
		eastl::map<eastl::string, eastl::shared_ptr<Content>> m_content;

		eastl::vector<eastl::pair<eastl::string, eastl::shared_ptr<Content>>> m_contentForLoad;

	};
}

#endif // !CONTENTMANAGER_H
