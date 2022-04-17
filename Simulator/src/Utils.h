#pragma once

#include <string>
#include <sstream>
#include <Windows.h>

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

	template<typename T>
	T Clamp(T a, T min, T max)
	{
		assert(max >= min);

		if (a < min)
			return min;
		if (a > max)
			return max;

		return a;
	}

	template<typename T>
	T Min(T a, T b)
	{
		if (a <= b)
			return a;

		return b;
	}

	template<typename T>
	T Max(T a, T b)
	{
		if (a >= b)
			return a;

		return b;
	}

	char* ReadFromPipe(HANDLE fileHandle, bool waitForInput);

}