#include "pch.h"
#include "common/logger.h"

namespace engine
{
	void Logger::init()
	{
		m_fileLogger = spdlog::basic_logger_st("hackathon", "hackathon_log.txt");
	}
}
