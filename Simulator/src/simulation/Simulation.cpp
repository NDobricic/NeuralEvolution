#include "../Core.h"
#include "../Configuration.h"
#include "Creature.h"

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

		LOG_INFO("Starting the simulation...");
		std::vector<std::shared_ptr<Creature>> creatures;
		creatures.reserve(ConfigManager::Settings().numCreatures);
		
		for (int i = 0; i < ConfigManager::Settings().numCreatures; i++)
		{
			std::shared_ptr<Creature> a
				= std::make_shared<Creature>(Creature(	Color(255, 255, 0), 100, 100, 0,
														ConfigManager::Settings().outputPath + "/" + std::to_string(i)));

			creatures.push_back(a);
		}

		int cycleNumber = 0;
		while (cycleNumber < 20)
		{
			for (auto c : creatures)
			{
				c->SimulateCycle();
			}

			cycleNumber++;
		}


	}
}