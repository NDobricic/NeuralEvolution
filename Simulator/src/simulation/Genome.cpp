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

		//for (int i = 0; i < length; i++)
		//	genome.genes.push_back(std::make_shared<Gene>(Gene::Random()));
		for (int i = 0; i < length; i++)
			genome.genes.push_back(std::make_shared<Gene>(Gene()));

		return genome;
	}

	Genome Genome::Combine(const Genome& a, const Genome& b, float dotMutationChance, float numAberrChance)
	{
		const Genome& longer = (a.Length() >= b.Length()) ? a : b;
		const Genome& shorter = (a.Length() < b.Length()) ? a : b;

		int length = longer.Length();

		Genome genome(longer.Length());
		for (int i = 0; i < shorter.Length(); i++)
		{
			std::shared_ptr<Gene> gene;
			if(Random::Next<int>(0, 1))
				gene = std::make_shared<Gene>(*a.genes[i]);
			else
				gene = std::make_shared<Gene>(*b.genes[i]);

			float dotRand = Random::NextReal(0.0f, 1.0f);

			if (dotMutationChance > dotRand)
				gene->Mutate();

			genome.genes.push_back(gene);
		}

		for (int i = shorter.Length(); i < longer.Length(); i++)
		{
			genome.genes.push_back(std::make_shared<Gene>(*longer.genes[i]));
		}

		float numRand = Random::NextReal(0.0f, 1.0f);
		if (numRand < numAberrChance)
		{
			genome.genes.push_back(std::make_shared<Gene>(Gene::Random()));
		}
		else if (numRand > 1.0f - numAberrChance && genome.Length() > 0)
		{
			int randIndex = Random::Next(0, genome.Length() - 1);
			genome.genes.erase(genome.genes.begin() + randIndex);
		}

		return genome;
	}
}