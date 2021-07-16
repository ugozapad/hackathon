#include "pch.h"
#include "engine/enginetask.h"

#include "common/mallocallocator.h"
#include "common/thread.h"

#include <thread>
#include <EASTL/atomic.h>

namespace engine
{
	static eastl::atomic<int> g_needToHarakiriThreads = 0;
	static MallocAllocator g_threadAllocator;

	struct TaskThreadData
	{
		TaskManager* m_taskManager;
		int m_threadId;
	};

	class TaskThread : public Thread
	{
	public:
		TaskThread(TaskThreadData& data)
		{
			m_taskData = data;
		}

		void Execute() override
		{
			char buffer[256];
			sprintf(buffer, "Task thread #%i", m_taskData.m_threadId);
			setThreadName(buffer);

			spdlog::info("spawned task thread #{}", m_taskData.m_threadId);

			while (g_needToHarakiriThreads.load() == 0)
			{


				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			spdlog::info("exiting task thread #{}", m_taskData.m_threadId);
		}

	private:
		TaskThreadData m_taskData;

	};

	static eastl::fixed_vector<TaskThread, 256> g_threads;


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
			TaskThreadData threadData;
			threadData.m_threadId = i;
			threadData.m_taskManager = this;

			g_threads.push_back(TaskThread(threadData));
			g_threads.back().startThread();
		}
	}

	void TaskManager::destroyTaskWorkers()
	{
		g_needToHarakiriThreads.store(1);

		for (int i = 0; i < g_threads.size(); i++)
		{
			TaskThread& taskThread = g_threads[i];
			if (taskThread.IsRunning())
				taskThread.stopThread();
		}
	}

}
