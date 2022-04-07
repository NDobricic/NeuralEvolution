#include "Core.h"

#include "simulation/Simulation.h"

int main(int argc, char* argv[])
{
	evol::Log::Init();

	evol::StartSimulation(argc, argv);

	return 0;
}