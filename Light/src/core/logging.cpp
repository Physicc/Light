#include "core/logging.hpp"

namespace Light
{
	std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

	void Logger::init()
	{
		spdlog::set_pattern("***%n*** [%H:%M:%S %z] [%P:%t] %v");

		s_coreLogger = spdlog::stderr_color_mt("Light");
		s_coreLogger->set_level(spdlog::level::trace);
		s_clientLogger = spdlog::stderr_color_mt("Client");
		s_clientLogger->set_level(spdlog::level::trace);
	}
}