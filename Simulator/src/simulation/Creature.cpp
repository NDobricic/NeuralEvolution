#include "Creature.h"
#include "../Configuration.h"
#include "spdlog/fmt/ostr.h"
#include <random>
#include "MapData.h"

namespace evol
{
	Creature::Creature(const std::string& outputPath, int globalIndex, int32_t startCycle, const Genome& genome, Color col, int16_t x, int16_t y)
		: genome(genome), color(col), posX(x), posY(y), outputPath(outputPath), globalIndex(globalIndex)
	{
		MapData::OccupyCell(posX, posY);

		WriteData(startCycle);
		WriteData(col.R);
		WriteData(col.G);
		WriteData(col.B);

		inputToInternalConn = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, INPUT_NEURONS);
		inputToOutputConn = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, INPUT_NEURONS);
		internalConn = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, INTERNAL_NEURONS);
		internalToOutputConn = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, INTERNAL_NEURONS);

		inputVals = Eigen::MatrixXf::Zero(INPUT_NEURONS, 1);
		internalVals = Eigen::MatrixXf::Zero(INTERNAL_NEURONS, 1);
		outputVals = Eigen::MatrixXf::Zero(OUTPUT_NEURONS, 1);

		ParseGenes();

		inputVals[0] = 1.0f;
	}

	void Creature::ParseGenes()
	{
		for (int i = 0; i < genome.Length(); i++)
		{
			if (genome.genes[i]->sourceType == 0 && genome.genes[i]->sinkType == 1) // input to internal
			{
				uint16_t in = genome.genes[i]->sourceIndex % INPUT_NEURONS;
				uint16_t out = genome.genes[i]->sinkIndex % INTERNAL_NEURONS;
				inputToInternalConn(out, in) = genome.genes[i]->valueAsFloat();
			}
			else if (genome.genes[i]->sourceType == 1 && genome.genes[i]->sinkType == 0) // internal to output
			{
				uint16_t in = genome.genes[i]->sourceIndex % INTERNAL_NEURONS;
				uint16_t out = genome.genes[i]->sinkIndex % OUTPUT_NEURONS;
				internalToOutputConn(out, in) = genome.genes[i]->valueAsFloat();
			}
			else if (genome.genes[i]->sourceType == 0 && genome.genes[i]->sinkType == 0) // input to output
			{
				uint16_t in = genome.genes[i]->sourceIndex % INPUT_NEURONS;
				uint16_t out = genome.genes[i]->sinkIndex % OUTPUT_NEURONS;
				inputToOutputConn(out, in) = genome.genes[i]->valueAsFloat();
			}
			else // internal to internal
			{
				uint16_t in = genome.genes[i]->sourceIndex % INTERNAL_NEURONS;
				uint16_t out = genome.genes[i]->sinkIndex % INTERNAL_NEURONS;
				internalConn(out, in) = genome.genes[i]->valueAsFloat();
			}
		}
	}

	void Creature::SimulateCycle()
	{
		inputVals[1] = ((float)posX / ConfigManager::Settings().mapSizeX - 0.5f) * 2.0f;
		inputVals[2] = ((float)posY / ConfigManager::Settings().mapSizeY - 0.5f) * 2.0f;

		int16_t minX = INT16_MAX;
		int16_t minY = INT16_MAX;
		int32_t minDist = INT32_MAX;
		int closestFoodIndex = 0;
		size_t numFood = MapData::food.size();

		for (int i = 0; i < numFood; i++)
		{
			int16_t diffX = MapData::food[i]->x - posX;
			int16_t diffY = MapData::food[i]->y - posY;
			int32_t dist = diffX * diffX + diffY * diffY;

			if (dist < minDist)
			{
				minDist = dist;
				minX = diffX;
				minY = diffY;
				closestFoodIndex = i;

				MapData::food[i]->x - posX;
				MapData::food[i]->y - posY;
			}
		}

		if (minDist <= 16)
		{
			health += 30.0f;
			MapData::RemoveFood(closestFoodIndex);
		}

		if (minX == 0)
			inputVals[3] = 0;
		else
			inputVals[3] = (minX > 0) ? utils::Min(1.0f, 10.0f / minX) : utils::Max(-1.0f, 10.0f / minX);

		if (minY == 0)
			inputVals[4] = 0;
		else
			inputVals[4] = (minY > 0) ? utils::Min(1.0f, 10.0f / minY) : utils::Max(-1.0f, 10.0f / minY);

		internalVals = inputToInternalConn * inputVals + internalConn * internalVals;
		outputVals = inputToOutputConn * inputVals + internalToOutputConn * internalVals;

		if (Sigmoid(outputVals(0)) > 0.5)
			Move(+1, 0);
		else if (Sigmoid(outputVals(0)) < -0.5)
			Move(-1, 0);

		if (Sigmoid(outputVals(1)) > 0.5)
			Move(0, +1);
		else if (Sigmoid(outputVals(1)) < -0.5)
			Move(0, -1);


		WriteData(posX);
		WriteData(posY);

		age++;
		health -= 1.0f;
	}

	bool Creature::Move(int moveX, int moveY)
	{
		int16_t newX = utils::Clamp<int>((int)posX + moveX, 0, ConfigManager::Settings().mapSizeX - 1);
		int16_t newY = utils::Clamp<int>((int)posY + moveY, 0, ConfigManager::Settings().mapSizeY - 1);

		if (MapData::IsCellOccupied(newX, newY))
			return false;

		MapData::FreeCell(posX, posY);
		posX = newX;
		posY = newY;
		MapData::OccupyCell(posX, posY);

		return true;
	}

	bool Creature::IsDead()
	{
		return health <= 0.0f;
	}

	std::shared_ptr<Creature> Creature::Combine(const Creature& a, const Creature& b, int globalIndex)
	{
		Genome genome = Genome::Combine(a.genome, b.genome,
			ConfigManager::Settings().dotMutationChance, ConfigManager::Settings().numericAberrationChance);
		Color color = Color::Combine(a.color, b.color);

		auto result = std::make_shared<Creature>(a.outputPath, globalIndex, MapData::CurrentCycle(), genome, color);
		return result;
	}

	int Creature::Age()
	{
		return age;
	}

	Creature::~Creature()
	{
		MapData::FreeCell(posX, posY);

		std::ofstream file(outputPath + "/" + std::to_string(globalIndex), std::ios::binary);

		file.write(binaryData.data(), binaryData.size());

		file.close();
	}
}