#include "../Core.h"
#include "../Configuration.h"
#include "Creature.h"

namespace evol
{
	
	void StartSimulation(int argc, char* argv[])
	{
		std::string configName = (argc < 2) ? "../config.ini" : argv[1];

		LOG_INFO("Establishing a connection between GUI and engine...");
		HANDLE fileHandle = CreateFileW(TEXT("\\\\.\\pipe\\evolpipe")
			, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		char* msg = utils::ReadFromPipe(fileHandle, true);

		if (strcmp(msg, "success") == 0)
		{
			LOG_INFO("Successfully connected the pipe.");
		}
		else
		{
			LOG_FATAL("Failed to connect the communication pipe between GUI and simulation. Exiting...");
			return;
		}

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

		LOG_TRACE("Initializing {0} creatures...", ConfigManager::Settings().numCreatures);
		int num = ConfigManager::Settings().numCreatures;
		std::vector<std::shared_ptr<Creature>> creatures;
		creatures.reserve(num);

		for (int i = 0; i < num; i++)
		{
			std::vector<Gene> genome;
			//genome.push_back(Gene(0, 0, 1, 1, 12000));
			//genome.push_back(Gene(1, 1, 0, 2, 2000));
			for(int j = 0; j<32; j++)
				genome.push_back(Gene());


			std::shared_ptr<Creature> a
				= std::make_shared<Creature>(	genome, Color::Random(), 100, 100, 0,
												ConfigManager::Settings().outputPath + "/" + std::to_string(i));

			creatures.push_back(a);
		}

		int cycleNumber = 0;
		bool running = true;

		LOG_TRACE("Starting the main simulation loop...");
		while (running)
		{
			if(cycleNumber % 10 == 0)
				LOG_TRACE("Cycle {0}", cycleNumber);

			for (int i = 0; i < ConfigManager::Settings().numCreatures; i++)
			{
				creatures[i]->SimulateCycle();
			}

			if (strcmp(utils::ReadFromPipe(fileHandle, false), "stop") == 0)
				running = false;

			cycleNumber++;
		}

		std::ofstream file(ConfigManager::Settings().outputPath + "/info");
		file.write(reinterpret_cast<char*>(&cycleNumber), sizeof(cycleNumber));
		file.flush();
	}
}