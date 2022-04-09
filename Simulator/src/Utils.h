#pragma once

#include <string>
#include <sstream>

namespace utils
{
	template<typename T>
	bool TryParseString(const std::string& input, T& output)
	{
		std::istringstream iss(input);

		iss >> std::noskipws >> output;

		return iss.eof() && !iss.fail();

		return false;
	}

}