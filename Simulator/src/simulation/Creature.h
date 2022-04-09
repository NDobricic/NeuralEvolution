#pragma once

#include "../Core.h"
#include "Color.h"
#include <fstream>

namespace evol
{
	class Creature
	{
	private:
		Color color;
		int16_t posX, posY;

		std::ofstream file;

	public:
		Creature(Color col, int16_t x, int16_t y, int32_t startCycle, const std::string& outputPath);

		const Color& GetColor() const { return color; }
		void SimulateCycle();
		void SaveOutput();
	};
}
