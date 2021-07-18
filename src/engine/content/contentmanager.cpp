#include "pch.h"
#include "engine/content/contentmanager.h"

#include "common/thread.h"

namespace engine
{
	static eastl::atomic<int> g_harakiriContentThread = 0;
	static eastl::atomic<int> g_needToLoadContent = 0;
	static std::mutex g_contentMutex;

	class ContentThread : public Thread
	{
	private:
		ContentManager* m_contentManager;

	public:
		void startContentThread(ContentManager* contentManager)
		{
			m_contentManager = contentManager;

			startThread();
		}

		void execute() override
		{
			setThreadName("Content thread");

			spdlog::info("Content thread is started!");

			while (g_harakiriContentThread.load() != 1)
			{
				if (g_needToLoadContent.load() == 1)
				{
					std::lock_guard<std::mutex> lockGuard(g_contentMutex);

					for (int i = 0; i < m_contentManager->m_contentForLoad.size(); i++)
					{
						eastl::shared_ptr<Content>& content = m_contentManager->m_contentForLoad[i].second;


					}
					
					g_needToLoadContent.store(0);
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}

	};

	ContentManager ContentManager::ms_instance;
	static ContentThread g_contentThread;

	void ContentManager::init()
	{
		g_contentThread.startContentThread(this);
	}

	void ContentManager::shutdown()
	{
		g_harakiriContentThread.store(1);
		g_contentThread.stopThread();
	}

	eastl::shared_ptr<TextureMap> ContentManager::loadTexture(const eastl::string& textureName)
	{
		//auto textureMap = eastl::find(m_content.begin(), m_content.end(), texturename);
		auto textureMap = m_content.at(textureName);

		// we will guess content is not loaded for now
		//if (textureMap == m_content.end())
		if (!textureMap)
		{
			auto content = eastl::make_shared<TextureMap>();
			m_contentForLoad.push_back(eastl::make_pair(textureName, content));

			// lock thread


			// and wait for finish ...
			while (g_needToLoadContent.load() == 1);
		}

		return eastl::static_shared_pointer_cast<TextureMap>(textureMap);
	}

}
