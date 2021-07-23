#include "pch.h"
#include "common/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/msvc_sink.h"

namespace engine
{
	void Logger::init()
	{
		spdlog::logger* logger = spdlog::default_logger_raw();

		// clear sinks
		logger->sinks().clear();

		// add file sink
		logger->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("engine_log.txt", true));

		// add console sink
		logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());

		// add msvc debuf sink
		logger->sinks().push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());

		logger->flush_on(spdlog::level::info);
	}
}
