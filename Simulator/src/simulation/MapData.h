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
		static std::ofstream file;

	public:
		static std::vector<std::shared_ptr<Coord<uint16_t>>> food;
		static int cycleNum;

		static void Init(const std::string& outputFile)
		{
			file.open(outputFile, std::ios::binary);
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

		static void SaveData()
		{
			uint32_t numFood = (uint32_t)food.size();

			file.write(reinterpret_cast<char*>(&numFood), sizeof(numFood));

			for (int i = 0; i < numFood; i++)
			{
				file.write(reinterpret_cast<char*>(&food[i]->x), sizeof(food[i]->x));
				file.write(reinterpret_cast<char*>(&food[i]->y), sizeof(food[i]->y));
			}

			file.flush();
		}
	};
}