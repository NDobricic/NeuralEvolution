#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace evol
{
	Config ConfigManager::configObj;

	bool ConfigManager::LoadFromFile(const std::string& configFile)
	{
		std::ifstream file(configFile);

		if (file.fail())
		{
			LOG_ERROR("Failed to open the config file.");
			return false;
		}

		int lineNum = -1;
		while (!file.eof())
		{
			lineNum++;

			std::string line;
			std::getline(file, line);

			if (line[0] == '#')
				continue;

			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

			if (line.length() == 0)
				continue;

			size_t equalsPos = line.find('=');
			if (equalsPos == std::string::npos)
			{
				LOG_WARN("Failed to load parameter at line {0}: '=' is missing.", lineNum);
				continue;
			}

			std::string paramName = line.substr(0, equalsPos);
			std::string paramValue = line.substr(equalsPos + 1);

			if (paramValue == "true") paramValue = "1";
			else if (paramValue == "false") paramValue = "0";

			int32_t paramInt; uint32_t paramUInt;
			uint16_t paramUShort;
			float paramFloat; bool paramBool;

			bool isInt = utils::TryParseString(paramValue, paramInt);
			bool isUInt = utils::TryParseString(paramValue, paramUInt);
			bool isUShort = utils::TryParseString(paramValue, paramUShort);
			bool isFloat = utils::TryParseString(paramValue, paramFloat);
			bool isBool = utils::TryParseString(paramValue, paramBool);

			if (paramName == "numCreatures" && isUShort) configObj.numCreatures = paramUShort;
			else if (paramName == "mapSizeX" && isUShort) configObj.mapSizeX = paramUShort;
			else if (paramName == "mapSizeY" && isUShort) configObj.mapSizeY = paramUShort;
			else if (paramName == "outputPath") configObj.outputPath = paramValue;
			else
			{
				LOG_WARN("Unable to load parameter '{0}' at line {1}.", paramName, lineNum);
			}
		}

		return true;
	}

	const Config& ConfigManager::Settings()
	{
		return configObj;
	}

	std::string ConfigManager::PrintSettings()
	{
		std::stringstream stream;
		stream << "numCreatures = " << configObj.numCreatures << "\n";
		stream << "mapSizeX = " << configObj.mapSizeX << "\n";
		stream << "mapSizeY = " << configObj.mapSizeY << "\n";
		stream << "outputPath = " << configObj.outputPath << "\n";
		stream.flush();


		return stream.str();
	}
}