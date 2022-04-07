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
			int paramValue = std::stoi(line.substr(equalsPos + 1));

			if (paramName == "numCreatures") configObj.numCreatures = paramValue;
			else if (paramName == "mapSizeX") configObj.mapSizeX = paramValue;
			else if (paramName == "mapSizeY") configObj.mapSizeY = paramValue;
			else
			{
				LOG_WARN("Unknown parameter '{0}' at line {1}.", paramName, lineNum);
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
		stream.flush();


		return stream.str();
	}
}