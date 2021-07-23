#ifndef ENGINETASK_H
#define ENGINETASK_H

namespace engine
{
	class EngineTask
	{
	public:
		EngineTask(std::function<void()>& function);
	};

	class TaskManager : public Singleton<TaskManager>
	{
	private:
		static TaskManager ms_instance;

	public:
		void addTask(EngineTask& task);

		void createTaskWorkers();
		void destroyTaskWorkers();

	private:
		std::vector<EngineTask> m_tasks;
	};
}

#endif // !ENGINETASK_H
