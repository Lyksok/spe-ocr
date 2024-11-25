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

double softmax(int K, double mat[1][K]);

double sigmoid(double x);

double dSigmoid(double x);

double random_w_b();

void init_weights(int row, int col, double array[row][col]);

void forward(int i, double training_inputs[][numInputs]);

void change_weights(const double lr, double deltaOutput[numOutputs], 
                        double deltaHidden[numHiddenNodes]);

void backward(int i, const double lr, double training_output[][numOutputs]);

void print_res(double training_inputs[][numInputs], 
                        double training_output[][numOutputs]);

void train(int numEpochs, double training_inputs[numTrainingSets][numInputs]
                    double training_output[numTrainingSets][numOutput]);
