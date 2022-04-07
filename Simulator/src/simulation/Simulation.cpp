#include "../Core.h"
#include "../Configuration.h"

namespace evol
{
	void StartSimulation(int argc, char* argv[])
	{
		std::string configName = (argc < 2) ? "config.ini" : argv[1];
		
		LOG_INFO("Loading settings...");
		bool success = ConfigManager::LoadFromFile(configName);
		if (!success)
		{
			LOG_FATAL("Unable to initialize settings from '{0}'. Aborting the simulation...", configName);
			return;
		}
		else
		{
			LOG_INFO("Successfully loaded settings from {0}.", configName);
		}

		LOG_TRACE("Current settings:\n{0}", ConfigManager::PrintSettings());
	}
}