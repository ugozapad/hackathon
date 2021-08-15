#ifndef CORE_H
#define CORE_H

namespace engine
{
	class Core
	{
	public:
		static void msg(const char* msg, ...);
		static void error(const char* msg, ...);
		static void warning(const char* msg, ...);
	};
}

#endif // !CORE_H
