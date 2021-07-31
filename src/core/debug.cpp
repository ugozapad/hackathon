#include "pch.h"
#include "debug.h"

#ifdef _MSC_VER
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

#include <Process.h>
#include <imagehlp.h>
#include <new.h>

namespace engine
{
	LONG WINAPI UnhandleExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		char executableName[256];
		GetModuleFileNameA(NULL, executableName, 256);

		char minidumpPath[256];
		sprintf(minidumpPath, "%s_%i-%i-%i_%i-%i-%i-%i.dmp",
			executableName, time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		HANDLE minidumpFile = CreateFileA(minidumpPath, GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (minidumpFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION minidumpInfo;
			minidumpInfo.ThreadId = GetCurrentThreadId();
			minidumpInfo.ExceptionPointers = ExceptionInfo;
			minidumpInfo.ClientPointers = FALSE;

			DWORD minidumpType = MiniDumpNormal;

			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), minidumpFile,
				(MINIDUMP_TYPE)minidumpType, &minidumpInfo, 0, 0);

			CloseHandle(minidumpFile);
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}

	int __cdecl out_of_memory_handler(size_t size)
	{
		Core::error("Out of Memory! Failed to allocate %u bytes", size);
	}

	static struct DebugInitializer
	{
		DebugInitializer()
		{
			Debug::Initialize();
		}
	} sDebugInitializer;

	void Debug::Initialize()
	{
		_set_new_mode(1);
		_set_new_handler(out_of_memory_handler);

		SetUnhandledExceptionFilter(UnhandleExceptionFilter);

		char exePath[MAX_PATH];
		GetModuleFileNameA(NULL, exePath, MAX_PATH);

		bool ret = SymInitialize(GetCurrentProcess(), exePath, FALSE);
		if (ret)
		{
			GetModuleFileNameA(NULL, exePath, MAX_PATH);
			if (!SymLoadModule(GetCurrentProcess(), NULL, exePath, NULL, 0, 0))
			{
				SymCleanup(GetCurrentProcess());
			}
		}
	}
#else
void Debug::Initialize()
{

}
#endif // _MSC_VER

void Debug::AssertionFailed(const char* expression, const char* file, int line, const char* function)
{
	static char buffer[2048];

	//strcat(buffer, "Assertion Failed!\n");

	strcat(buffer, "Expression: ");
	strcat(buffer, expression);
	strcat(buffer, "\n");

	strcat(buffer, "File: ");
	strcat(buffer, file);
	strcat(buffer, "\n");

	char b[16];
	sprintf(b, "%i", line);

	strcat(buffer, "Line: ");
	strcat(buffer, b);
	strcat(buffer, "\n");

	strcat(buffer, "Function: ");
	strcat(buffer, function);

	AssertionFailedBackend(buffer);
}

void Debug::AssertionFailed(const char* expression, const char* file, int line, const char* function, const char* message)
{
	static char buffer[2048];

	strcat(buffer, "Assertion Failed!\n");

	strcat(buffer, "Expression: ");
	strcat(buffer, expression);
	strcat(buffer, "\n");

	strcat(buffer, "File: ");
	strcat(buffer, file);
	strcat(buffer, "\n");

	char b[16];
	sprintf(b, "%i", line);

	strcat(buffer, "Line: ");
	strcat(buffer, b);
	strcat(buffer, "\n");

	strcat(buffer, "Function: ");
	strcat(buffer, function);
	strcat(buffer, "\n");

	strcat(buffer, "Message: ");
	strcat(buffer, message);

	AssertionFailedBackend(buffer);
}

void Debug::AssertionFailedBackend(const char* buf)
{
	spdlog::error("--- ASSERT MESSAGE ---");
	spdlog::error("{}", buf);

	//#ifdef WIN32
	//	if (IsDebuggerPresent())
	//	{
	//		DebugBreak();
	//	}
	//	else
	//	{
	//		int result = MessageBoxA(0, buf, "Assertion failed", MB_CANCELTRYCONTINUE);
	//
	//		switch (result)
	//		{
	//		case IDCANCEL: exit(1); break;
	//		case IDTRYAGAIN: DebugBreak(); break;
	//		case IDCONTINUE: return;
	//		}
	//	}
	//#endif

#ifdef WIN32
	int result = MessageBoxA(0, buf, "Assertion failed", MB_CANCELTRYCONTINUE | MB_ICONERROR);

	switch (result)
	{
	case IDCANCEL: std::terminate(); break;
	case IDTRYAGAIN: DebugBreak(); break;
	case IDCONTINUE: return;
	}
#endif
}

}
