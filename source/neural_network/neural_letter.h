#include <stddef.h>
#pragma once

#define numInputs 2
#define numHiddenNodes 3
#define numOutputs 1

extern double hiddenLayer[numHiddenNodes];
extern double outputLayer[numOutputs];
extern double hiddenLayerBias[numHiddenNodes];
extern double outputLayerBias[numOutputs];
extern double hiddenWeights[numInputs][numHiddenNodes];
extern double outputWeights[numHiddenNodes][numOutputs];

double sigmoid(double x);

double dSigmoid(double x);

double init_weights();

void train();

void forward(int i);

void backward(int i);
