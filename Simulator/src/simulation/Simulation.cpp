#include "../Core.h"
#include "../Configuration.h"
#include "Creature.h"
#include "MapData.h"
#include <filesystem>

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

		LOG_INFO("Preparing the output directory...");
		std::filesystem::remove_all(ConfigManager::Settings().outputPath);
		std::filesystem::create_directory(ConfigManager::Settings().outputPath);

		LOG_INFO("Starting the simulation...");

		LOG_INFO("Initializing the map...");
		MapData::Init(ConfigManager::Settings().outputPath);

		LOG_INFO("Initializing {0} creatures...", ConfigManager::Settings().numCreatures);
		int num = ConfigManager::Settings().numCreatures;
		std::vector<std::shared_ptr<Creature>> creatures;
		creatures.reserve(num);

		for (int i = 0; i < num; i++)
		{
			std::shared_ptr<Creature> a
				= std::make_shared<Creature>(ConfigManager::Settings().outputPath, i, 0);

			creatures.push_back(a);
		}

		LOG_INFO("Spawning {0} food...", ConfigManager::Settings().numFood);
		for (int i = 0; i < ConfigManager::Settings().numFood; i++)
		{
			MapData::AddFood();
		}


		int totalCreatures = creatures.size();
		bool running = true;

		LOG_TRACE("Creating the statistics output file...");
		std::ofstream stats(ConfigManager::Settings().outputPath + "/stats", std::ios_base::binary);

		LOG_TRACE("Starting the main simulation loop...");
		while (running)
		{
			if(MapData::CurrentCycle() % 100 == 0)
				LOG_TRACE("Cycle {0}", MapData::cycleNum);

			MapData::SaveData();

			for (int i = 0; i < creatures.size(); i++)
			{
				if (creatures[i]->Age() >= 115)
				{
					int randIndex = Random::Next<int>(0, creatures.size() - 1);
					if (randIndex != i && creatures[randIndex]->Age() >= 115)
					{
						creatures.push_back(Creature::Combine(*creatures[i], *creatures[randIndex], totalCreatures));
						totalCreatures++;
						//LOG_TRACE("Combined creatures {0} and {1}", i, randIndex);
					}
				}

				if (creatures[i]->IsDead())
				{
					creatures.erase(creatures.begin() + i);
					i--;
				}
				else
				{
					creatures[i]->SimulateCycle();
				}
			}

			if (MapData::CurrentCycle() % 1 == 0)
			{
				for(int i = 0; i < 1; i++)
					MapData::AddFood();
			}

			MapData::cycleNum++;

			uint32_t numCreatures = (uint32_t)creatures.size();
			stats.write(reinterpret_cast<char*>(&numCreatures), sizeof(numCreatures));
			stats.flush();


			if (strcmp(utils::ReadFromPipe(fileHandle, false), "stop") == 0)
				running = false;
		}

		stats.flush();

		std::ofstream file(ConfigManager::Settings().outputPath + "/info", std::ios_base::binary);
		int cycleNum = MapData::CurrentCycle();
		file.write(reinterpret_cast<char*>(&cycleNum), sizeof(cycleNum));
		file.write(reinterpret_cast<char*>(&totalCreatures), sizeof(totalCreatures));
		file.flush();
	}
}