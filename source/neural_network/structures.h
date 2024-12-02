#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Neuron {
	// TODO
	double bias;
	double value;
} Neuron;

// to avoid struct for prev and next
typedef struct Layer Layer;
struct Layer {
	// TODO
	int numNeurons;
	Neuron *neurons;
	// equivalent to the number of inputs
	int numWeights;
	// a matrice where each row
	// is the weigths given to one neuron
	// it has dimension numNeurons * numWeights
	double **weights;
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
