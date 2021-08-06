#include "pch.h"
#include "engine/enginetask.h"

#include "common/mallocallocator.h"
#include "common/thread.h"

#include <thread>

namespace engine
{
	static std::atomic<int> g_needToHarakiriThreads = 0;

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

		void execute() override
		{
			char buffer[256];
			sprintf(buffer, "Task thread #%i", m_taskData.m_threadId);
			setThreadName(buffer);

			spdlog::info("spawned task thread #{}", m_taskData.m_threadId);

			TaskManager* taskManager = m_taskData.m_taskManager;
			assert(taskManager);

			while (g_needToHarakiriThreads.load() == 0)
			{
				std::vector<EngineTask> tasks = taskManager->m_tasks;
				EngineTask* task;

				if (!tasks.empty())
				{
					task = &tasks.back();
					
					std::function<void()>& taskFunc = task->get();
					taskFunc();

					tasks.pop_back();
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			spdlog::info("exiting task thread #{}", m_taskData.m_threadId);
		}

	private:
		TaskThreadData m_taskData;

	};

	static std::vector<TaskThread*> g_threads;


	//EngineTask::EngineTask(std::function<void()>& function)
	//{

	//}

	EngineTask::EngineTask(std::function<void()> function)
	{
		m_function = function;
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

			g_threads.push_back(new TaskThread(threadData));
			g_threads.back()->startThread();
		}
	}

	void TaskManager::destroyTaskWorkers()
	{
		g_needToHarakiriThreads.store(1);

		for (int i = 0; i < g_threads.size(); i++)
		{
			TaskThread* taskThread = g_threads[i];
			if (taskThread->IsRunning())
			{
				taskThread->stopThread();
				delete taskThread;
			}
		}
	}

}
