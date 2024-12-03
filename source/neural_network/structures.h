#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>

typedef struct Neuron {
	double bias;
	double value;
} Neuron;

// to avoid struct for prev and next
typedef struct Layer Layer;
struct Layer {
	int numNeurons;
	Neuron *neurons;
	// equivalent to the number of inputs
	int numWeights;
	// every inputs
	double *inputs;
	// a matrice where each row
	// is the weigths given to one neuron
	// it has dimension numNeurons * numWeights
	double **weights;
	Layer *prev;
	Layer *next;
};

typedef struct TrainingData {
	int size;
	int nbinputs;
	double **inputs;
	double **outputs;
	char *expected;
	double lr;
} TrainingData;

typedef struct Network {
	// pointer to the first layer of the network
	Layer layers;
	double *outputs;
	int nbsuccess;
	int nbruns;
} Network;

#endif /* STRUCTURES_H */
