#ifndef ENGINETASK_H
#define ENGINETASK_H

namespace engine
{
	class EngineTask
	{
	public:
		EngineTask(eastl::function<void()>& function);
	};

	class TaskManager : public Singleton<TaskManager>
	{
	private:
		static TaskManager ms_instance;

	public:
		void addTask(EngineTask& task);

		void createTaskWorkers();

	private:
		eastl::fixed_vector<EngineTask, 256> m_tasks;
	};
}

#endif // !ENGINETASK_H
