#include "pch.h"

#include "common/thread.h"

#ifdef WIN32

#include <process.h>

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType;	  // Must be 0x1000.
	LPCSTR szName;	  // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags;	  // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SetThreadName(DWORD dwThreadID, const char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable : 6320 6322)
	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
#pragma warning(pop)
}

namespace engine
{

void setCurrentThreadName(const char* name)
{
	::SetThreadName(GetCurrentThreadId(), name);
}

void Thread::setThreadName(const char* name)
{
	::SetThreadName(GetCurrentThreadId(), name);
}

void Thread::stopThread()
{
	if (!m_isRunning)
		return;

	m_isRunning = false;

	WaitForSingleObject(m_threadHandle, INFINITE);
	CloseHandle(m_threadHandle);
}

static unsigned threadProc(void* data)
{
	Thread* thread = reinterpret_cast<Thread*>(data);
	thread->Execute();
	_endthreadex(0);
	return 0;
}

bool Thread::startThread()
{
	m_threadId = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadProc, this, 0, 0);
	m_threadHandle = (HANDLE)m_threadId;

	m_isRunning = true;

	return true;
}

#endif

Thread::Thread()
	: m_threadHandle(NULL),
	  m_threadId(0),
	  m_isRunning(false)
{
}

Thread::~Thread()
{
	if (m_isRunning)
		stopThread();
}

}