#include "Log.h"

#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace evol
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Simulation.log", true));

		logSinks[0]->set_pattern("[%T] %^[%l] %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("SIMULATION", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);
	}
}