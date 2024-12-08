#ifndef NEURAL_LETTER_H
#define NEURAL_LETTER_H

#include <stddef.h>

#include "structures.h"

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

void init_weights(int row, int col, double array[row][col]);
/* PROBLEMS
void init_weights(double array[row][col]);
double init_weights();
*/

void train();

void forward(int i, double training_inputs[numTrainingSets][numInputs]);
// void forward(int i);

void backward(int i);

#endif
