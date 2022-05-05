#include "MapData.h"

namespace evol
{
	uint16_t MapData::sizeX;
	uint16_t MapData::sizeY;

	std::vector<std::shared_ptr<Creature>> MapData::creatures;
	std::vector<std::shared_ptr<Food>> MapData::food;
	std::vector<bool> MapData::feromones;

	std::ofstream MapData::foodFile;
	std::ofstream MapData::feromoneFile;
	std::vector<std::shared_ptr<MapObject>> MapData::objects;

	int MapData::cycleNum;
}