#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Neuron {
	// TODO
	double weight;
	double bias;
} Neuron;

typedef struct Layer {
	// TODO
	int nodesIn;
	int nodesOut;
	Neuron *neurons;
} Layer;

typedef struct TrainingData {
	// TODO
	double *expected;
} TrainingData;

typedef struct Network {
	// TODO
	int numLayers;
	Layer *layers;
	double **inputs;
	double **outputs;
} Network;

#endif /* STRUCTURES_H */
