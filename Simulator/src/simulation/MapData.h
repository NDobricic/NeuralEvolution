#pragma once

#include "../Core.h"
#include "../Configuration.h"
#include <fstream>

namespace evol
{
	template<typename T>
	struct Coord
	{
		T x, y;

		Coord() : x(0), y(0) { }
		Coord(T x, T y) : x(x), y(y) { }
	};

	class MapData
	{
	private:
		static std::ofstream foodFile;
		static std::ofstream feromoneFile;

	public:
		static std::vector<std::shared_ptr<Coord<uint16_t>>> food;
		static std::vector<bool> feromones;

		static std::vector<uint16_t> occupation;
		static int cycleNum;

		static void Init(const std::string& outputDir)
		{
			foodFile.open(outputDir + "/food", std::ios::binary);
			feromoneFile.open(outputDir + "/feromones", std::ios::binary);

			feromones.resize(ConfigManager::Settings().mapSizeX * ConfigManager::Settings().mapSizeY);
			std::fill(feromones.begin(), feromones.end(), false);

			occupation.resize(ConfigManager::Settings().mapSizeX * ConfigManager::Settings().mapSizeY);
			std::fill(occupation.begin(), occupation.end(), 0);

			cycleNum = 0;
		}

		static int CurrentCycle() { return cycleNum; }

		static void AddFood()
		{
			uint16_t x = Random::Next<uint16_t>(0u, ConfigManager::Settings().mapSizeX - 1);
			uint16_t y = Random::Next<uint16_t>(0u, ConfigManager::Settings().mapSizeY - 1);

			food.push_back(std::make_shared<Coord<uint16_t>>(x, y));
		}

		static void RemoveFood(int index)
		{
			food.erase(food.begin() + index);
		}

		static void SetFeromone(int x, int y, bool value = true)
		{
			feromones[y * ConfigManager::Settings().mapSizeX + x] = value;
		}

		static bool GetFeromone(int x, int y)
		{
			return feromones[y * ConfigManager::Settings().mapSizeX + x];
		}

		static void OccupyCell(int x, int y)
		{
			occupation[y * ConfigManager::Settings().mapSizeX + x]++;
		}

		static void FreeCell(int x, int y)
		{
			if (occupation[y * ConfigManager::Settings().mapSizeX + x] > 0)
				occupation[y * ConfigManager::Settings().mapSizeX + x]--;
		}

		static bool IsCellOccupied(int x, int y)
		{
			return occupation[y * ConfigManager::Settings().mapSizeX + x] > 0;
		}

		static void SaveData()
		{
			uint32_t numFood = (uint32_t)food.size();

			foodFile.write(reinterpret_cast<char*>(&numFood), sizeof(numFood));

			for (int i = 0; i < numFood; i++)
			{
				foodFile.write(reinterpret_cast<char*>(&food[i]->x), sizeof(food[i]->x));
				foodFile.write(reinterpret_cast<char*>(&food[i]->y), sizeof(food[i]->y));
			}

			//utils::WriteBoolVecToFile(feromoneFile, feromones);



			foodFile.flush();
		}
	};
}