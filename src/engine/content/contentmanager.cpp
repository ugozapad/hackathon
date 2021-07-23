#include "pch.h"
#include "engine/content/contentmanager.h"

#include "file/filestream.h"

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

			spdlog::info("Content thread is started");

			while (g_harakiriContentThread.load() != 1)
			{
				if (g_needToLoadContent.load() == 1)
				{
					std::lock_guard<std::mutex> lockGuard(g_contentMutex);

					for (int i = 0; i < m_contentManager->m_contentForLoad.size(); i++)
					{
						eastl::string& contentName = m_contentManager->m_contentForLoad[i].first;
						spdlog::info("[content]: load {}", contentName.c_str());

						eastl::shared_ptr<Content>& content = m_contentManager->m_contentForLoad[i].second;
						eastl::shared_ptr<DataStream> dataStream = eastl::make_shared<FileStream>(contentName, "r");

						content->load(dataStream);
					}

					m_contentManager->m_contentForLoad.clear();

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

		m_content.clear();
	}

	eastl::shared_ptr<TextureMap> ContentManager::loadTexture(const eastl::string& textureName)
	{
		auto textureMap = m_content.find(textureName);

		// we will guess content is not loaded for now
		if (textureMap == m_content.end())
		{
			auto content = eastl::make_shared<TextureMap>(textureName);

			// lock thread for add info about content loading
			g_contentMutex.lock();
			m_contentForLoad.push_back(eastl::make_pair(textureName, content));
			g_contentMutex.unlock();

			// lock thread for loading
			g_needToLoadContent.store(1);

			// wait for finish ...
			while (g_needToLoadContent.load() == 1);

			// because gl is shit we need to this magic
			eastl::static_shared_pointer_cast<TextureMap>(content)->createHWTexture();

			// little magic
			m_content.emplace(textureName, content);
		}

		textureMap = m_content.find(textureName);
		return eastl::static_shared_pointer_cast<TextureMap>((*textureMap).second);
	}

	eastl::shared_ptr<ModelBase> ContentManager::loadModel(const eastl::string& modelName)
	{
		auto model = m_content.find(modelName);

		// we will guess content is not loaded for now
		if (model == m_content.end())
		{
			auto content = eastl::make_shared<ModelBase>(modelName);

			// lock thread for add info about content loading
			g_contentMutex.lock();
			m_contentForLoad.push_back(eastl::make_pair(modelName, content));
			g_contentMutex.unlock();

			// lock thread for loading
			g_needToLoadContent.store(1);

			// wait for finish ...
			while (g_needToLoadContent.load() == 1);

			// because our content manager is shit we need to this magic
			eastl::static_shared_pointer_cast<ModelBase>(content)->createHwShit();

			// little magic
			m_content.emplace(modelName, content);
		}

		model = m_content.find(modelName);
		return eastl::static_shared_pointer_cast<ModelBase>((*model).second);
	}

	eastl::shared_ptr<Material> ContentManager::loadMaterial(const eastl::string& materialName)
	{
		auto material = m_content.find(materialName);

		// we will guess content is not loaded for now
		if (material == m_content.end())
		{
			auto content = eastl::make_shared<Material>(materialName);

			// lock thread for add info about content loading
			g_contentMutex.lock();
			m_contentForLoad.push_back(eastl::make_pair(materialName, content));
			g_contentMutex.unlock();

			// lock thread for loading
			g_needToLoadContent.store(1);

			// wait for finish ...
			while (g_needToLoadContent.load() == 1);

			// because gl is shit we need to this magic
			eastl::static_shared_pointer_cast<Material>(content)->createHwTextures();

			// little magic
			m_content.emplace(materialName, content);
		}

		material = m_content.find(materialName);
		return eastl::static_shared_pointer_cast<Material>((*material).second);
	}
}
