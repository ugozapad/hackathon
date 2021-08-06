#ifndef ENGINETASK_H
#define ENGINETASK_H

namespace engine
{
	class EngineTask
	{
	public:
		EngineTask(std::function<void()> function);

		void operator()() 
		{
			m_function(); 
		}

		std::function<void()>& get() { return m_function; }

	private:
		std::function<void()> m_function;
	};

	class TaskThread;

	class TaskManager : public Singleton<TaskManager>
	{
		friend class TaskThread;
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
