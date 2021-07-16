#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace engine
{
	class Logger
	{
	public:
		void init();

		void shutdown();

	private:
		std::shared_ptr<spdlog::logger> m_consoleLogger;
		std::shared_ptr<spdlog::logger> m_fileLogger;
	};


}

#endif // !LOGGER_H
