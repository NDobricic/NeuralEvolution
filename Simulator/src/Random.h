#pragma once

#include <random>
#include <cstdint>

class Random
{
private:
	static std::random_device rd;
	static std::mt19937_64 engine;

public:
	template<typename T>
	static T Next(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(engine);
	}
};