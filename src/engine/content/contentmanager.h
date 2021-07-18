#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "engine/content/content.h"

#include "graphics/texturemap.h"

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

	private:
		eastl::map<eastl::string, eastl::shared_ptr<Content>> m_content;

		eastl::vector<eastl::pair<eastl::string, eastl::shared_ptr<Content>>> m_contentForLoad;

	};
}

#endif // !CONTENTMANAGER_H
