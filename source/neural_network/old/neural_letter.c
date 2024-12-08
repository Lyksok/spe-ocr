#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "neural_letter.h"
#include "structures.h"

#define numTrainingSets 4
/* OK SO DEFINE IS FOR CONSTANT ONLY
// no idea i'm juste trying to not have errors
#define training_inputs[numTrainingSets][numInputs]
#define training_output[numTrainingSets][numOutputs]
#define deltaOutput[numOutputs]
#define deltaHidden[numberOfEpochs]
*/
// rajouter une fonction softmax ?

double sigmoid(double x) { return 1 / (1 + exp(-x)); }

double dSigmoid(double x) { return x * (1 - x); }

double random() { return ((double)rand()) / ((double)RAND_MAX); }

void init_weights(int row, int col, double array[row][col]) {

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      array[i][j] = random();
    }
  }
}

void forward(int i, double training_inputs[numTrainingSets][numInputs]) {
  for (int j = 0; j < numHiddenNodes; j++) {
    double activation = hiddenLayerBias[j];

    for (int k = 0; k < numInputs; k++) {
      activation += training_inputs[i][k] * hiddenWeights[k][j];
    }

    hiddenLayer[j] = sigmoid(activation);
  }

  for (int j = 0; j < numOutputs; j++) {
    double activation = hiddenLayerBias[j];
    for (int k = 0; k < numHiddenNodes; k++) {
      activation += hiddenLayer[k] * outputWeights[k][j];
    }
    outputLayer[j] = sigmoid(activation);
  }
}

void change_weights(const double lr) {

  for (int j = 0; j < numOutputs; j++) {
    outputLayerBias[j] += deltaOutput[j] * lr;
    for (int k = 0; k < numHiddenNodes; k++) {
      outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
    }
  }

  for (int j = 0; j < numHiddenNodes; j++) {
    hiddenLayerBias[j] += deltaHidden[j] * lr;
    for (int k = 0; k < numInputs; k++) {
      hiddenWeights[k][j] += hiddenLayer[k] * deltaHidden[j] * lr;
    }
  }
}

void backward(const double lr) {

  double deltaOutput[numOutputs];
  for (int j = 0; j < numOutputs; j++) {
    double error = (training_output[i][j] - outputLayer[j]);
    deltaOutput[j] = error * dSigmoid(outputLayer[j]);
  }

  // Compute change in hidden weights
  double deltaHidden[numHiddenNodes];
  for (int j = 0; j < numHiddenNodes; j++) {
    double error = 0.0f;
    for (int k = 0; k < numOutputs; k++) {
      error += deltaOutput[k] * outputWeights[j][k];
    }
    deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
  }

  change_weights(lr);
}

int main(void) {
  const double lr = 0.1f;

  // pourquoi une liste et pas une matrice ?
  double training_inputs[numTrainingSets][numInputs] = {
      {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
  // a transformer en une liste (A, B, C...)
  double training_output[numTrainingSets][numOutputs] = {
      {1.0f}, {0.0f}, {0.0f}, {1.0f}};

  // Init of weights
  init_weights(numInputs, numHiddenNodes,
               hiddenWeights[numInputs][numHiddenNodes]);
  init_weights(numHiddenNodes, numOutputs,
               outputWeights[numHiddenNodes][numOutputs]);

  // Init of bias
  for (int i = 0; i < numOutputs; i++) {
    outputLayerBias[i] = random();
  }

  int numberOfEpochs = 100000;

  for (int epoch = 0; epoch < numberOfEpochs; epoch++) {

    for (int i = 0; i < numTrainingSets; i++) {
      // Forward pass --> fonction a part
      forward(i);
      // meriterait *presque* sa fonction x)
      printf("Input: [%g, %g] Output: %g Predicted Output: %g \n",
             training_inputs[i][0], training_inputs[i][1], outputLayer[0],
             training_output[i][0]);

      // Backwardpropagation --> fonction a part
      backward(lr);
    }
  }
}
