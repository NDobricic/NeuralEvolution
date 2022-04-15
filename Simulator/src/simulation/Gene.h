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

	Gene()
	{
		sourceType = Random::Next<uint16_t>(0,1);
		sourceIndex = Random::Next<uint16_t>(0, 0x7fff);
		sinkType = Random::Next<uint16_t>(0, 1);
		sinkIndex = Random::Next<uint16_t>(0, 0x7fff);
		value = Random::Next<int16_t>(-0x7fff, 0x7fff);
	}

	Gene(uint16_t sourceType, uint16_t sourceIndex, uint16_t sinkType, uint16_t sinkIndex, int16_t value)
		: sourceType(sourceType), sourceIndex(sourceIndex), sinkType(sinkType), sinkIndex(sinkIndex), value(value)
	{ }
};