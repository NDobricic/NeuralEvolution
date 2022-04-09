#include "Creature.h"
#include "../Configuration.h"

namespace evol
{
	Creature::Creature(Color col, int16_t x, int16_t y, int32_t startCycle, const std::string& outputPath)
		: color(col), posX(x), posY(y), file(outputPath, std::ios::binary)
	{
		file.write(reinterpret_cast<char*>(&startCycle), sizeof(startCycle));
		file << col.R << col.G << col.B;
	}

	void Creature::SimulateCycle()
	{
		posX = (posX + 1) % ConfigManager::Settings().mapSizeX;

		file.write(reinterpret_cast<char*>(&posX), sizeof(posX));
		file.write(reinterpret_cast<char*>(&posY), sizeof(posY));
	}

	void Creature::SaveOutput()
	{
		file.flush();
	}
}