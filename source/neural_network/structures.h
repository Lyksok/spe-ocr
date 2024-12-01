#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Neuron {
	// TODO
	double weight;
	double bias;
	double output;
} Neuron;

typedef struct Layer {
	// TODO
	/*
	int numNodesIn;
	Neuron *nodesIn;
	int numNodesOut;
	Neuron *nodesOut;
	*/
	int numNeurons;
	Neuron *neurons;
	int numWeights;
	double *weights;
	struct Layer *prev;
	struct Layer *next;
} Layer;

typedef struct TrainingData {
	// TODO
	double **inputs;
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
