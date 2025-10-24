#include "utils.h"
#include <vector>

Network::Network(std::string p_typeOfBlocks, std::vector<int> p_countOfNeuronsOnLayers) 
	: typeOfBlocks(p_typeOfBlocks), countOfNeuronsOnLayers(p_countOfNeuronsOnLayers);

Network::Network() : typeOfBlocks("NegTau"), countOfNeuronsOnLayers({2, 2});

void Network::calcOutputOfLayer(int numOfLayer) {
	std::vector<double> intermediateVector = multiplyMatrixByVector(weightsOfLayers[numOfLayer], inputOutputVector);
	inputOutputVector.clear();
	
	for (int i = 0; i < neuronsOfLayers[numOfLayer].size(); i++) {
		std::vector<double> initialState = henkamonoInitialStates[i].insert(henkamonoInitialStates.begin(), intermediateVector[i]);
		neuronsOfLayers[numOfLayer][i].init_blocks(initialState);
		neuronsOfLayers[numOfLayer][i].work(typeOfBlocks, typeOfBlocks, false);
		double outputOfNeuron = neuronsOfLayers[numOfLayer][i].henkamonoOutput;
		inputOutputVector.push_back(outputOfNeuron);
	}
}

std::vector<double> Network::work() {
	for (int i = 0; i < weightsOfLayers.size(); i++) {
		calcOutputOfLayer(i);
	}
	return inputOutputVector;
}

void Network::createNeurons(int numOfLayer) {
	for (int i = 0; i < neuronsOfLayers[numOfLayer].size(); i++) {
		Henkamono neuron;
		neuron.init_blocks(initStates[numOfLayer][i]);
		neuronsOfLayers[numOfLayer].push_back();
	}
}

void Network::train() {}