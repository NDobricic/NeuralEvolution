#include "Genome.h"

namespace evol
{
	Genome::Genome(int length)
	{
		genes.reserve(length);
	}

	int Genome::Length() const
	{
		return genes.size();
	}

	Genome Genome::Random(int length)
	{
		Genome genome(length);

		for (int i = 0; i < length; i++)
			genome.genes.push_back(std::make_shared<Gene>(Gene::Random()));

		return genome;
	}

	Genome Genome::Combine(const Genome& a, const Genome& b)
	{
		assert(a.Length() == b.Length());

		int length = a.Length();
		Genome genome(length);
		for (int i = 0; i < length; i++)
		{
			if(Random::Next<int>(0, 1))
				genome.genes.push_back(std::make_shared<Gene>(*a.genes[i]));
			else
				genome.genes.push_back(std::make_shared<Gene>(*b.genes[i]));
		}

		return genome;
	}
}