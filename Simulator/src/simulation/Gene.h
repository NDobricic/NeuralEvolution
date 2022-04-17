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

	void Mutate()
	{
		int bit = Random::Next<int>(0, 31);

		if (bit >= 0 && bit < 16)
			value = value ^ (1u << bit);
		else if (bit >= 16 && bit < 23)
			sinkIndex = sinkIndex ^ (1u << (bit - 16));
		else if (bit == 23)
			sinkType = sinkType ^ 1u;
		else if (bit >= 24 && bit < 31)
			sourceIndex = sourceIndex ^ (1u << (bit - 24));
		else
			sourceType = sourceType ^ 1u;
	}

	//Gene(const Gene& original)
	//{
	//	sourceType = original.sourceType;
	//	sourceIndex = original.sourceIndex;
	//	sinkType = original.sinkType;
	//	sinkIndex = original.sinkIndex;
	//	value = original.value;
	//}

	Gene()
	{
		sourceType = 0;
		sourceIndex = 0;
		sinkType = 1;
		sinkIndex = 0;
		value = Random::Next<int16_t>(-0x7fff, 0x7fff);
	}

	Gene(uint16_t sourceType, uint16_t sourceIndex, uint16_t sinkType, uint16_t sinkIndex, int16_t value)
		: sourceType(sourceType), sourceIndex(sourceIndex), sinkType(sinkType), sinkIndex(sinkIndex), value(value)
	{ }
};