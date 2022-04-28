#include "Core.h"

#include "simulation/Simulation.h"

int main(int argc, char* argv[])
{
	_setmaxstdio(2048); //TODO: find a better solution than this

	evol::Log::Init();

	evol::StartSimulation(argc, argv);

	return 0;
}