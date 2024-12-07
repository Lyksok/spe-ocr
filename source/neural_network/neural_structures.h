#ifndef NEURAL_STRUCTURES_H
#define NEURAL_STRUCTURES_H

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
	// is the weights given to one neuron
	// it has dim numNeurons * numWeights
	double **weights;
	Layer *prev;
	Layer *next;
};

typedef struct TrainingData TrainingData;
struct TrainingData {
	double *inputs;
	char expected;
	TrainingData *next;
};

typedef struct Network {
	Layer layers;
	double **outputs;
	double lr;
} Network;

#endif /* NEURAL_STRUCTURES_H */
