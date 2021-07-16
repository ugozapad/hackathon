#include "pch.h"
#include "engine/enginetask.h"
#include "common/mallocallocator.h"

#include <thread>
#include <EASTL/atomic.h>

namespace engine
{
	static eastl::atomic<int> g_needToHarakiriThreads = 0;
	static eastl::fixed_vector<std::thread::id, 256> g_threadIds;
	static MallocAllocator g_threadAllocator;

	struct TaskThreadData
	{
		TaskManager* m_taskManager;
		int m_threadId;
	};

	static void TaskThreadProc(TaskThreadData* data)
	{
		spdlog::info("spawned task thread #{}", data->m_threadId);

		while (g_needToHarakiriThreads.load() == 0)
		{
			

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		spdlog::info("exiting task thread #{}", data->m_threadId);
		mem_delete(*g_sysAllocator, data);
	}

	EngineTask::EngineTask(eastl::function<void()>& function)
	{

	}

	TaskManager TaskManager::ms_instance;

	void TaskManager::addTask(EngineTask& task)
	{
		m_tasks.push_back(task);
	}

	void TaskManager::createTaskWorkers()
	{
		uint32_t threadCount = std::thread::hardware_concurrency();
		spdlog::info("Total {} cores", threadCount);

		for (int i = 0; i < threadCount; i++)
		{
// 			TaskThreadData* threadData = mem_new<TaskThreadData>(*g_sysAllocator);
// 			threadData->m_taskManager = this;
// 			threadData->m_threadId = i;
// 
// 			std::thread thread(&TaskThreadProc, threadData);
// 			thread.detach();
// 
// 			g_threadIds.push_back(thread.get_id());
		}
	}

	void TaskManager::destroyTaskWorkers()
	{
		//g_needToHarakiriThreads.store(1);
	}

}
