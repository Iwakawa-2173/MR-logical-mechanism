#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "henkamono.h"

class Network {
public:
	
	std::string typeOfBlocks;
	
	std::vector<std::vector<std::vector<double>>> weightsOfLayers;
	std::vector<double> inputOutputVector;
	std::vector<std::vector<Henkamono>> neuronsOfLayers;
	std::vector<std::vector<std::vector<double>>> initStates
	
	std::vector<int> countOfNeuronsOnLayers;
	
	Network(std::string p_typeOfBlocks, std::vector<int> p_countOfNeuronsOnLayers);
	Network();
	
	void calcOutputOfLayer(int numOfLayer);
	
	std::vector<double> work();
	
	void createAndInitNeurons(int numOfLayer);
	
	void train();
	
}

#endif // NETWORK_H
