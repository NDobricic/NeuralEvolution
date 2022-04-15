#pragma once

#define INPUT_NEURONS 3
#define INTERNAL_NEURONS 10
#define OUTPUT_NEURONS 2

#include "../Core.h"
#include "Color.h"
#include <fstream>
#include <Core>
#include "Gene.h"

namespace evol
{
	class Creature
	{
	private:
		Color color;
		int16_t posX, posY;

		std::ofstream file;

		Eigen::Matrix<float, INTERNAL_NEURONS, INPUT_NEURONS> inputToInternalConn;
		Eigen::Matrix<float, OUTPUT_NEURONS, INPUT_NEURONS> inputToOutputConn;
		Eigen::Matrix<float, INTERNAL_NEURONS, INTERNAL_NEURONS> internalConn;
		Eigen::Matrix<float, OUTPUT_NEURONS, INTERNAL_NEURONS> internalToOutputConn;

		Eigen::Matrix<float, INPUT_NEURONS, 1> inputVals;
		Eigen::Matrix<float, INTERNAL_NEURONS, 1> internalVals;
		Eigen::Matrix<float, OUTPUT_NEURONS, 1> outputVals;

		std::vector<Gene> genome;

		void ParseGenes();

		inline float Sigmoid(float x)
		{
			return x / (1 + std::abs(x));
		}

	public:
		Creature(std::vector<Gene> genes, Color col, int16_t x, int16_t y, int32_t startCycle, const std::string& outputPath);

		const Color& GetColor() const { return color; }
		void SimulateCycle();
		void SaveOutput();
	};
}
