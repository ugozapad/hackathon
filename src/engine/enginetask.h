#ifndef ENGINETASK_H
#define ENGINETASK_H

namespace engine
{
	class EngineTask
	{
	public:
		EngineTask(eastl::function<void()>& function);
	};

	class EngineTaskManager : public Singleton<EngineTaskManager>
	{
	private:
		static EngineTaskManager ms_instance;

	public:
		void addTask(EngineTask& task);

		void createTaskWorkers();

	private:
		static void TaskThreadProc(EngineTaskManager* taskManager);

	private:
		eastl::fixed_vector<EngineTask, 256> m_tasks;
	};
}

#endif // !ENGINETASK_H
