#pragma once

#include "../Core.h"
#include "Gene.h"

namespace evol
{
	class Genome
	{
	public:
		std::vector<std::shared_ptr<Gene>> genes;

		Genome(int length);
		int Length() const;
		static Genome Random(int length);
		static Genome Combine(const Genome& a, const Genome& b, float mutationChance = 0.0f, float numAberrChance = 0.0f);
	};
}