#include "readMatSpikeData.h"
#include <iostream>

int main(void) {

	readMatSpikeData *readData = new readMatSpikeData("D:/dev/Data/Spikes_center_out_data_HOTS.mat",  "spikedata");
	std::cout << "Starting" << std::endl;
	std::cout << readData->getNumSpikes(0) << std::endl;
	//std::cout << readData->getStartTime(0) << std::endl;
	//std::cout << readData->getEndTime(0) << std::endl;
	
	std::vector<std::array<double, 3>> x = readData->getPositions(0);

	std::cout << "Trial Labels:" << std::endl;
	for (int i = 0; i < readData->getTotalTrials(); i++)
	{
		std::cout << readData->getTrialLabel(i) << std::endl;
	}

	std::cout << "Ending" << std::endl;
	std::cin.get();
	return(1);
}