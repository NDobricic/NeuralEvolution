#pragma once

#define SPDLOG_LEVEL_NAMES  { "TRACE", "DEBUG", "INFO",  "WARNING", "ERROR", "FATAL", "OFF" };
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop)

namespace evol
{
	class Log
	{
	public:
		static void Init();


		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

#define LOG_TRACE(...)    evol::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     evol::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     evol::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    evol::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)    evol::Log::GetCoreLogger()->critical(__VA_ARGS__)