#include "MapData.h"

namespace evol
{
	std::vector<std::shared_ptr<Coord<uint16_t>>> MapData::food;
	std::ofstream MapData::file;
	int MapData::cycleNum;
}