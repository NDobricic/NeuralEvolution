#include "Creature.h"
#include "../Configuration.h"
#include "spdlog/fmt/ostr.h"
#include <random>

namespace evol
{
	Creature::Creature(std::vector<Gene> genes, Color col, int16_t x, int16_t y, int32_t startCycle, const std::string& outputPath)
		: genome(std::move(genes)), color(col), posX(x), posY(y), file(outputPath, std::ios::binary)
	{
		file.write(reinterpret_cast<char*>(&startCycle), sizeof(startCycle));
		file << col.R << col.G << col.B;

		inputToInternalConn = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, INPUT_NEURONS);
		inputToOutputConn = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, INPUT_NEURONS);
		internalConn = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, INTERNAL_NEURONS);
		internalToOutputConn = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, INTERNAL_NEURONS);

		inputVals = Eigen::MatrixXf::Zero(INPUT_NEURONS, 1);
		internalVals = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, 1);
		outputVals = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, 1);

		posX = Random::Next(0, ConfigManager::Settings().mapSizeX - 1);
		posY = Random::Next(0, ConfigManager::Settings().mapSizeY - 1);

		ParseGenes();

		inputVals[0] = 1.0f;
	}

	void Creature::ParseGenes()
	{
		for (int i = 0; i < genome.size(); i++)
		{
			if (genome[i].sourceType == 0 && genome[i].sinkType == 1) // input to internal
			{
				uint16_t in = genome[i].sourceIndex % INPUT_NEURONS;
				uint16_t out = genome[i].sinkIndex % INTERNAL_NEURONS;
				inputToInternalConn(out, in) = genome[i].valueAsFloat();
			}
			else if (genome[i].sourceType == 1 && genome[i].sinkType == 0) // internal to output
			{
				uint16_t in = genome[i].sourceIndex % INTERNAL_NEURONS;
				uint16_t out = genome[i].sinkIndex % OUTPUT_NEURONS;
				internalToOutputConn(out, in) = genome[i].valueAsFloat();
			}
			else if (genome[i].sourceType == 0 && genome[i].sinkType == 0) // input to output
			{
				uint16_t in = genome[i].sourceIndex % INPUT_NEURONS;
				uint16_t out = genome[i].sinkIndex % OUTPUT_NEURONS;
				inputToOutputConn(out, in) = genome[i].valueAsFloat();
			}
			else // internal to internal
			{
				uint16_t in = genome[i].sourceIndex % INTERNAL_NEURONS;
				uint16_t out = genome[i].sinkIndex % INTERNAL_NEURONS;
				internalConn(out, in) = genome[i].valueAsFloat();
			}
		}
	}

	void Creature::SimulateCycle()
	{
		inputVals[1] = ((float)posX / ConfigManager::Settings().mapSizeX - 0.5f) * 2.0f;
		inputVals[2] = ((float)posY / ConfigManager::Settings().mapSizeY - 0.5f) * 2.0f;

		internalVals = inputToInternalConn * inputVals + internalConn * internalVals;
		outputVals = inputToOutputConn * inputVals + internalToOutputConn * internalVals;

		if(Sigmoid(outputVals(0)) > 0.5)
			posX = (posX + 1u) % ConfigManager::Settings().mapSizeX;
		else if (Sigmoid(outputVals(0)) < -0.5)
			posX = (posX - 1u) % ConfigManager::Settings().mapSizeX;

		if (Sigmoid(outputVals(1)) > 0.5)
			posY = (posY + 1u) % ConfigManager::Settings().mapSizeY;
		else if (Sigmoid(outputVals(1)) < -0.5)
			posY = (posY - 1u) % ConfigManager::Settings().mapSizeY;

		file.write(reinterpret_cast<char*>(&posX), sizeof(posX));
		file.write(reinterpret_cast<char*>(&posY), sizeof(posY));
	}

	void Creature::SaveOutput()
	{
		file.flush();
	}
}