#include "pch.h"
#include "core.h"

namespace engine
{
	void error_backend(const char* txt)
	{
#ifdef WIN32
		MessageBoxA(0, txt, "Engine Error", MB_OK | MB_ICONERROR);
#endif
	}

	void Core::msg(const char* msg, ...)
	{
		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		spdlog::info("{}", buf);
	}

	void Core::error(const char* msg, ...)
	{
		static char buf[2048 * 2];
		va_list args;

		va_start(args, msg);
		vsnprintf(buf, sizeof(buf), msg, args);
		va_end(args);

		spdlog::error("{}", buf);
		error_backend(buf);

//#if defined(DEBUG) || defined(_DEBUG)
//		std::terminate();
//#else
//		exit(1);
//#endif

		ExitProcess(1);
	}
}