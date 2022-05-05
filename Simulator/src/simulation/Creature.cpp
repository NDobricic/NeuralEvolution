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
		while (MapData::IsCellOccupied(posX, posY))
		{
			posX = Random::Next<uint16_t>(0, ConfigManager::Settings().mapSizeX - 1);
			posY = Random::Next<uint16_t>(0, ConfigManager::Settings().mapSizeY - 1);
		}

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
			auto foodPos = MapData::food[i]->GetPosition();
			int16_t diffX = foodPos.x - posX;
			int16_t diffY = foodPos.y - posY;
			int32_t dist = diffX * diffX + diffY * diffY;

			if (dist < minDist)
			{
				minDist = dist;
				minX = diffX;
				minY = diffY;
				closestFoodIndex = i;
			}
		}

		if (minDist <= 16)
		{
			health += 30.0f;
			MapData::RemoveFood(closestFoodIndex);
		}

		UpdateAxis(1, 0, 3, 4);
		UpdateAxis(1, 1, 5, 6);
		UpdateAxis(0, 1, 7, 8);
		UpdateAxis(0, -1, 9, 10);

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

	void Creature::UpdateAxis(int dx, int dy, int proximityNeuron, int colorNeuron)
	{
		auto obj = GetClosestObjOnAxis(dx, dy);
		if (obj != nullptr)
		{
			auto pos = obj->GetPosition();
			inputVals[proximityNeuron] = CalculateProximity(pos.x, pos.y);

			auto col = obj->GetColor();
			inputVals[colorNeuron] = 0.299 * col.R + 0.587 * col.G + 0.114 * col.B;
		}
		else
		{
			inputVals[proximityNeuron] = 0;
			inputVals[colorNeuron] = 0;
		}
	}

	std::shared_ptr<MapObject> Creature::GetClosestObjOnAxis(int dx, int dy)
	{
		int xpos = posX + dx;
		int ypos = posY + dy;

		int xneg = posX - dx;
		int yneg = posY - dy;

		std::shared_ptr<MapObject> obj(nullptr);
		while ((MapData::IsInsideBounds(xpos, ypos) || MapData::IsInsideBounds(xneg, yneg)) && obj == nullptr)
		{
			obj = MapData::GetObjAt(xpos, ypos);
			if (obj != nullptr)
				break;

			obj = MapData::GetObjAt(xneg, yneg);

			xpos += dx;
			ypos += dy;
			xneg -= dx;
			yneg -= dy;
		}

		return obj;
	}

	float Creature::CalculateProximity(int x, int y)
	{
		float result = 0.0f;
		float dist = sqrt((x - posX) * (x - posX) + (y - posY) * (y - posY));

		if (dist > 0.5f)
			result = (dist > 0) ? utils::Min(1.0f, 10.0f / dist) : utils::Max(-1.0f, 10.0f / dist);

		return result;
	}

	bool Creature::Move(int moveX, int moveY)
	{
		int16_t newX = utils::Clamp<int>((int)posX + moveX, 0, ConfigManager::Settings().mapSizeX - 1);
		int16_t newY = utils::Clamp<int>((int)posY + moveY, 0, ConfigManager::Settings().mapSizeY - 1);

		if (MapData::IsCellOccupied(newX, newY))
			return false;

		MapData::MoveObjectTo(this, newX, newY);
		posX = newX;
		posY = newY;

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
		Color color = Random::ChooseRandom(a.color, b.color);

		auto result = std::make_shared<Creature>(a.outputPath, globalIndex, MapData::CurrentCycle(), genome, color);
		return result;
	}

	Coord<uint16_t> Creature::GetPosition() const
	{
		return Coord<uint16_t>(posX, posY);
	}

	int Creature::Age()
	{
		return age;
	}

	Creature::~Creature()
	{
		std::ofstream file(outputPath + "/" + std::to_string(globalIndex), std::ios::binary);

		file.write(binaryData.data(), binaryData.size());

		file.close();
	}
}