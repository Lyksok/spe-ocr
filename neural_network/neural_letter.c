#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "neural_letter.h"

#define numTrainingSets 4
// rajouter une fonction softmax ?

double sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double dSigmoid(double x)
{
	return x * (1 - x);
}

double random_w_b()
{
	return ((double)rand()) / ((double)RAND_MAX);
}

void init_weights(int row, int col, double array[row][col])
{

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			array[i][j] = random_w_b();
		}
	}
}

void forward(int i, double training_inputs[][numInputs])
{
    for(int j = 0; j < numHiddenNodes; j++)
    {
        double activation = hiddenLayerBias[j];

        for(int k = 0; k < numInputs; k++)
        {
            activation += training_inputs[i][k] * hiddenWeights[k][j];
        }

        hiddenLayer[j] = sigmoid(activation);

    }

    for(int j = 0; j < numOutputs; j++)
    {
        double activation = hiddenLayerBias[j];
        for(int k = 0; k < numHiddenNodes; k++)
        {
            activation += hiddenLayer[k] * outputWeights[k][j];
        }
        outputLayer[j] = sigmoid(activation);
    }
}

void change_weights(const double lr, double deltaOutput[numOutputs], double deltaHidden[numHiddenNodes])
{
    
    for(int j = 0; j < numOutputs; j++)
    {
        outputLayerBias[j] += deltaOutput[j] * lr;
        for(int k = 0; k < numHiddenNodes; k++)
        {
            outputWeights[k][j] += hiddenLayer[k] *
                    deltaOutput[j] * lr;
        }
    }

    for(int j = 0; j < numHiddenNodes; j++)
    {
        hiddenLayerBias[j] += deltaHidden[j] * lr;
        for(int k = 0; k < numInputs; k++)
        {
            hiddenWeights[k][j] += hiddenLayer[k] *
                    deltaHidden[j] * lr;
        }
    }
}

void backward(int i, const double lr, double training_output[][numOutputs])
{

    double deltaOutput[numOutputs];
    for(int j = 0; j < numOutputs; j++)
    {
        double error = (training_output[i][j] - outputLayer[j]);
            deltaOutput[j] = error * dSigmoid(outputLayer[j]);
    }
    
    //Compute change in hidden weights
    double deltaHidden[numHiddenNodes];
    for(int j = 0; j < numHiddenNodes; j++)
    {
        double error = 0.0f;
        for(int k = 0; k < numOutputs; k++)
        {
            error += deltaOutput[k] * outputWeights[j][k];
        }
        deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
    }
    change_weights(lr, deltaOutput, deltaHidden);
}

void print_res(double training_inputs[][numInputs], 
                double training_output[][numOutputs])
{
    
     for(int i = 0; i < numTrainingSets; i++)
     {
        printf("Input: [%g, %g] Output: %g Predicted Output: %g \n",
            training_inputs[i][0], training_inputs[i][1],
            outputLayer[0], training_output[i][0]);
    }
}

void train(int numEpochs)
{
    const double lr = 0.1f; 
    double training_inputs[numTrainingSets][numInputs] = {
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f}
	};
	// a transformer en une liste (A, B, C...)
	double training_output[numTrainingSets][numOutputs] = {
		{1.0f},
		{0.0f},
		{0.0f},
		{1.0f}
	};

    for (int epoch = 0; epoch < numEpochs; epoch++)
    {
        for (int i = 0; i < numTrainingSets; i++)
        {
            forward(i, training_inputs);
            backward(i, lr, training_output);
        }
    }
    print_res(training_inputs, training_output);
}

int main(void)
{ 
	// Init of weights
    init_weights(numInputs, numHiddenNodes,
                    hiddenWeights);
    init_weights(numHiddenNodes, numOutputs,
                    outputWeights);
    // Init of bias
	for(int i = 0; i < numOutputs; i++)
	{
		outputLayerBias[i] = random_w_b();
	}

	int numberOfEpochs = 100000;
    
    train(numberOfEpochs);
    return 0;
}
