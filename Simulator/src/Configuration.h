#pragma once

#include "Core.h"

namespace evol
{
	struct Config
	{
		uint16_t numCreatures;
		uint16_t mapSizeX;
		uint16_t mapSizeY;

		uint16_t numFood;

		float dotMutationChance;
		float numericAberrationChance;

		std::string outputPath;
	};

	class ConfigManager
	{
	private:
		static Config configObj;

	public:
		static bool LoadFromFile(const std::string& configFile);
		static const Config& Settings();
		static std::string PrintSettings();
	};
}