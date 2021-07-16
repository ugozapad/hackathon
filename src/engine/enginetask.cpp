#include "pch.h"
#include "engine/enginetask.h"

#include <thread>

namespace engine
{
	EngineTask::EngineTask(eastl::function<void()>& function)
	{

	}

	EngineTaskManager EngineTaskManager::ms_instance;

	void EngineTaskManager::addTask(EngineTask& task)
	{
		m_tasks.push_back(task);
	}

	void EngineTaskManager::createTaskWorkers()
	{
		uint32_t threadCount = std::thread::hardware_concurrency();
		spdlog::info("Total {} cores", threadCount);

		for (int i = 0; i < threadCount; i++)
		{
			std::thread thread(&EngineTaskManager::TaskThreadProc, this);
			thread.detach();
		}
	}

	void EngineTaskManager::TaskThreadProc(EngineTaskManager* taskManager)
	{
		spdlog::info("spawned task thread");
	}

}
