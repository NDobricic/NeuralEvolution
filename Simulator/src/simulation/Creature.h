#pragma once

#define INPUT_NEURONS 5
#define INTERNAL_NEURONS 4
#define OUTPUT_NEURONS 2

#include "../Core.h"
#include "Color.h"
#include <fstream>
#include <Core>
#include "Genome.h"
#include "../Configuration.h"

namespace evol
{
	class Creature
	{
	private:
		Color color;
		int16_t posX, posY;
		float health = 100.0f;
		uint32_t age = 0;

		std::string outputPath;
		std::ofstream file;

		Eigen::Matrix<float, INTERNAL_NEURONS, INPUT_NEURONS> inputToInternalConn;
		Eigen::Matrix<float, OUTPUT_NEURONS, INPUT_NEURONS> inputToOutputConn;
		Eigen::Matrix<float, INTERNAL_NEURONS, INTERNAL_NEURONS> internalConn;
		Eigen::Matrix<float, OUTPUT_NEURONS, INTERNAL_NEURONS> internalToOutputConn;

		Eigen::Matrix<float, INPUT_NEURONS, 1> inputVals;
		Eigen::Matrix<float, INTERNAL_NEURONS, 1> internalVals;
		Eigen::Matrix<float, OUTPUT_NEURONS, 1> outputVals;

		Genome genome;

		void ParseGenes();

		inline float Sigmoid(float x)
		{
			return x / (1 + std::abs(x));
		}

	public:
		Creature(	const std::string& outputPath, int globalIndex, int32_t startCycle,
					const Genome& genome = Genome::Random(16), Color col = Color::Random(),
					int16_t x = Random::Next<uint16_t>(0, ConfigManager::Settings().mapSizeX - 1),
					int16_t y = Random::Next<uint16_t>(0, ConfigManager::Settings().mapSizeY - 1));

		~Creature();

		const Color& GetColor() const { return color; }
		void SimulateCycle();
		void SaveOutput();

		bool IsDead();
		int Age();

		static std::shared_ptr<Creature> Combine(const Creature& a, const Creature& b, int globalIndex);
	};
}
