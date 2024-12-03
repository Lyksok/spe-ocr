#ifndef NEURAL_LETTER_H
#define NEURAL_LETTER_H

#include <stddef.h>

#include "structures.h"

#define nLayers 2
#define dimensions 13
#define nTrainingSets 4

extern double *hiddenLayer;
// length = numHiddenNodes
extern double *outputLayer;
// length = numOutputs
extern double *hiddenLayerBias;
// length = numHiddenNodes
extern double *outputLayerBias;
// length = numOutputs
extern double **hiddenWeights;
// rows = numInputs - columns = numHiddenNodes
extern double **outputWeights;
// rows = numHiddenNodes - columns = numOutputs

void Forward(int length, Layer l, Network net);

void Backward(Network net, TrainingData data, int run);

void Result(Network net, TrainingData data, int nbrun);

void Train(int nbrun, Network net, TrainingData data);

#endif
