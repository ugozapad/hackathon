#ifndef THREAD_H
#define THREAD_H

namespace engine
{

#ifdef _WIN32
typedef unsigned ThreadId;
#else
// ???
#endif // _WIN32

class Thread
{
public:
	Thread();
	virtual ~Thread();

	bool startThread();
	void setThreadName(const char* name);

	void stopThread();

	virtual void execute() = 0;

	bool IsRunning() { return m_isRunning; }

private:
	void* m_threadHandle;
	ThreadId m_threadId;
	bool m_isRunning;
};

void setCurrentThreadName(const char* name);

} 

#endif