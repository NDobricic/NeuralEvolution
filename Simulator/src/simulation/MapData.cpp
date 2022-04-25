#include "MapData.h"

namespace evol
{
	std::vector<std::shared_ptr<Coord<uint16_t>>> MapData::food;
	std::vector<bool> MapData::feromones;
	std::vector<uint16_t> MapData::occupation;

	std::ofstream MapData::foodFile;
	std::ofstream MapData::feromoneFile;

	int MapData::cycleNum;
}