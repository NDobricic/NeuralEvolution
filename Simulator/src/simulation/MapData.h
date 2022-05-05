#pragma once

#include "../Core.h"
#include "../Configuration.h"
#include "Creature.h"
#include <fstream>
#include "MapObject.h"
#include "Food.h"

namespace evol
{
	enum class ObjectType
	{
		FOOD,
		CREATURE,
		OBSTACLE
	};

	class MapData
	{
	private:
		static uint16_t sizeX, sizeY;

		static std::ofstream foodFile;
		static std::ofstream feromoneFile;
		static std::vector<std::shared_ptr<MapObject>> objects;

	public:
		static std::vector<std::shared_ptr<Creature>> creatures;
		static std::vector<std::shared_ptr<Food>> food;
		static std::vector<bool> feromones;

		static int cycleNum;

		static void Init(uint16_t mapSizeX, uint16_t mapSizeY, int numCreatures, int numFood, const std::string& outputDir)
		{
			sizeX = mapSizeX;
			sizeY = mapSizeY;

			foodFile.open(outputDir + "/food", std::ios::binary);
			feromoneFile.open(outputDir + "/feromones", std::ios::binary);

			objects.resize(sizeX * sizeY);
			std::fill(objects.begin(), objects.end(), std::shared_ptr<MapObject>(nullptr));

			feromones.resize(sizeX * sizeY);
			std::fill(feromones.begin(), feromones.end(), false);

			LOG_INFO("Initializing {0} creatures...", numCreatures);
			for (int i = 0; i < numCreatures; i++)
			{
				std::shared_ptr<Creature> a
					= std::make_shared<Creature>(outputDir, i, 0);

				AddCreature(a);
			}

			LOG_INFO("Spawning {0} food...", numFood);
			for (int i = 0; i < numFood; i++)
			{
				AddFood();
			}

			cycleNum = 0;
		}

		static std::shared_ptr<MapObject> GetObjAt(int x, int y)
		{
			if(IsInsideBounds(x, y))
				return objects[y * sizeX + x];

			return std::shared_ptr<MapObject>(nullptr);
		}

		static int CurrentCycle() { return cycleNum; }

		static bool IsInsideBounds(int x, int y)
		{
			return (x >= 0 && x < sizeX && y > 0 && y < sizeY);
		}

		static void AddFood()
		{
			uint16_t x = Random::Next<uint16_t>(0u, sizeX - 1);
			uint16_t y = Random::Next<uint16_t>(0u, sizeY - 1);

			food.push_back(std::make_shared<Food>(x, y));
		}

		static void RemoveFood(int index)
		{
			food.erase(food.begin() + index);
		}

		static void SetFeromone(int x, int y, bool value = true)
		{
			feromones[y * sizeX + x] = value;
		}

		static bool GetFeromone(int x, int y)
		{
			return feromones[y * sizeX + x];
		}

		static void MoveObjectTo(const MapObject* obj, int x, int y)
		{
			auto pos = obj->GetPosition();
			auto objPtr = GetObjAt(pos.x, pos.y);

			objects[y * sizeX + x] = objPtr;
			objects[pos.y * sizeX + pos.x] = std::shared_ptr<MapObject>(nullptr);
		}

		static void DeleteObjAt(int x, int y)
		{
			objects[y * sizeX + x] = std::shared_ptr<MapObject>(nullptr);
		}

		static void RemoveObj(const MapObject* obj)
		{
			auto pos = obj->GetPosition();

			objects[pos.y * sizeX + pos.x] = std::shared_ptr<MapObject>(nullptr);
		}

		static void AddCreature(std::shared_ptr<Creature> creature)
		{
			auto pos = creature->GetPosition();
			
			objects[pos.y * sizeX + pos.x] = creature;
			creatures.push_back(creature);
		}

		static void RemoveCreature(int index)
		{
			auto pos = creatures[index]->GetPosition();

			creatures.erase(MapData::creatures.begin() + index);
			objects[pos.y * sizeX + pos.x] = std::shared_ptr<MapObject>(nullptr);
		}

		static bool IsCellOccupied(int x, int y)
		{
			bool isFree = (GetObjAt(x, y) == nullptr);
			return !isFree;
		}

		static void SaveData()
		{
			uint32_t numFood = (uint32_t)food.size();

			foodFile.write(reinterpret_cast<char*>(&numFood), sizeof(numFood));

			for (int i = 0; i < numFood; i++)
			{
				auto pos = food[i]->GetPosition();
				foodFile.write(reinterpret_cast<char*>(&pos.x), sizeof(pos.x));
				foodFile.write(reinterpret_cast<char*>(&pos.y), sizeof(pos.y));
			}

			//utils::WriteBoolVecToFile(feromoneFile, feromones);



			foodFile.flush();
		}
	};
}