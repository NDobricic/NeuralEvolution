#pragma once

#include "../Core.h"

struct Gene
{
	uint16_t sourceType		: 1;
	uint16_t sourceIndex	: 7;
	uint16_t sinkType		: 1;
	uint16_t sinkIndex		: 7;
	int16_t value;

	float valueAsFloat() const { return value / 8192.0f; }

	static Gene Random()
	{
		uint16_t sourceType = Random::Next<uint16_t>(0,1);
		uint16_t sourceIndex = Random::Next<uint16_t>(0, 0x7fff);
		uint16_t sinkType = Random::Next<uint16_t>(0, 1);
		uint16_t sinkIndex = Random::Next<uint16_t>(0, 0x7fff);
		int16_t value = Random::Next<int16_t>(-0x7fff, 0x7fff);

		return Gene(sourceType, sourceIndex, sinkType, sinkIndex, value);
	}

	//Gene(const Gene& original)
	//{
	//	sourceType = original.sourceType;
	//	sourceIndex = original.sourceIndex;
	//	sinkType = original.sinkType;
	//	sinkIndex = original.sinkIndex;
	//	value = original.value;
	//}

	Gene(uint16_t sourceType, uint16_t sourceIndex, uint16_t sinkType, uint16_t sinkIndex, int16_t value)
		: sourceType(sourceType), sourceIndex(sourceIndex), sinkType(sinkType), sinkIndex(sinkIndex), value(value)
	{ }
};