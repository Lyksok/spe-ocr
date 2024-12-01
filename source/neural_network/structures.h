#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Neuron {
	// TODO
	double weight;
	double bias;
	double output;
} Neuron;

// to avoid struct for prev and next
typedef struct Layer Layer;
struct Layer {
	// TODO
	int numNeurons;
	Neuron *neurons;
	int numWeights;
	double *weights;
	Layer *prev;
	Layer *next;
};

typedef struct TrainingData {
	// TODO
	int size;
	int nbinputs;
	double **inputs;
	char *expected;
	// the learning rate
	double lr;
} TrainingData;

typedef struct Network {
	// TODO
	int numLayers;
	Layer *layers;
	double **inputs;
	double **outputs;
} Network;

#endif /* STRUCTURES_H */
