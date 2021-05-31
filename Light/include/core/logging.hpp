#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "core/base.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Light
{
	class Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_coreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#ifndef NDEBUG
	#define LIGHT_CORE_TRACE(...)		Light::Logger::getCoreLogger()->trace(__VA_ARGS__)
	#define LIGHT_CORE_DEBUG(...)		Light::Logger::getCoreLogger()->debug(__VA_ARGS__)
	#define LIGHT_CORE_INFO(...)		Light::Logger::getCoreLogger()->info(__VA_ARGS__)
	#define LIGHT_CORE_WARN(...)		Light::Logger::getCoreLogger()->warn(__VA_ARGS__)
	#define LIGHT_CORE_ERROR(...)		Light::Logger::getCoreLogger()->error(_LIGHT_FILE() + ":"\
											_LIGHT_STRINGIFY_MACRO(__LINE__) "::" __VA_ARGS__)
	#define LIGHT_CORE_CRITICAL(...)	Light::Logger::getCoreLogger()->critical(_LIGHT_FILE() + ":"\
											_LIGHT_STRINGIFY_MACRO(__LINE__) "::" __VA_ARGS__)

	#define LIGHT_TRACE(...)			Light::Logger::getClientLogger()->trace(__VA_ARGS__)
	#define LIGHT_DEBUG(...)			Light::Logger::getClientLogger()->debug(__VA_ARGS__)
	#define LIGHT_INFO(...)				Light::Logger::getClientLogger()->info(__VA_ARGS__)
	#define LIGHT_WARN(...)				Light::Logger::getClientLogger()->warn(__VA_ARGS__)
	#define LIGHT_ERROR(...)			Light::Logger::getClientLogger()->error(_LIGHT_FILE() + ":"\
											_LIGHT_STRINGIFY_MACRO(__LINE__) "::" __VA_ARGS__)
	#define LIGHT_CRITICAL(...)			Light::Logger::getClientLogger()->critical(_LIGHT_FILE() + ":"\
											_LIGHT_STRINGIFY_MACRO(__LINE__) "::" __VA_ARGS__)
#else
	#define LIGHT_CORE_TRACE(...)
	#define LIGHT_CORE_DEBUG(...)
	#define LIGHT_CORE_INFO(...)
	#define LIGHT_CORE_WARN(...)
	#define LIGHT_CORE_ERROR(...)
	#define LIGHT_CORE_CRITICAL(...)

	#define LIGHT_TRACE(...)
	#define LIGHT_DEBUG(...)
	#define LIGHT_INFO(...)
	#define LIGHT_WARN(...)
	#define LIGHT_ERROR(...)
	#define LIGHT_CRITICAL(...)
#endif

#endif // __LOGGING_H__