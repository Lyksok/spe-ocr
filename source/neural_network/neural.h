#ifndef NEURAL_LETTER_H
#define NEURAL_LETTER_H

#include <stddef.h>

#include "structures.h"

#define numInputs 2
#define numHiddenNodes 3
#define numOutputs 1

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

void Softmax(int K, double *mat);

double sigmoid(double x);

double dSigmoid(double x);

void train();

void Sum(int length, double *inputs, Layer layer);

void Forward(int length, double *inputs, Layer l, double *outputs);

void Backward(Network net, TrainingData data, int run);

void Result(Network net, TrainingData data, int nbrun);

#endif
