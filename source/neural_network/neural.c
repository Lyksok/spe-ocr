#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "neural.h"
#include "structures.h"
#include "setup.h"

#define numTrainingSets 4

// to modify depending on how we're using it
void Softmax(int length, double *mat)
{
        double sum = 0;
        for (int n = 0; n < length; n++)
        {
		mat[n] = exp(mat[n]);
                sum += mat[n];
        }
        for (int n = 0; n < length; n++)
        {
                mat[n] = mat[n] / sum;
        }
}

double sigmoid(double x)
{
        return 1 / (1 + exp(-x));
}

// careful with the Vanishing gradient problem
double dSigmoid(double x)
{
        return x * (1 - x);
}

void Sum(int length, double *inputs, Layer layer)
{
	// foreach neurons
	for (int n = 0; layer.numNeurons; n++)
	{
		Neuron neuron = layer.neurons[n];
		// we initialize the value of the neuron
		//neuron.output = 0;
		double sum = 0;
		// foreach inputs
		for (int i = 0; i < length; i++)
		{
			double w = layer.weights[i];
			// we add the value times the weight
			//neuron.output += inputs[i] * w;
			sum += inputs[i] * w;
		}
		// then we add the bias
		//neuron.output += neuron.bias;
		sum += neuron.bias;
		neuron.output = sigmoid(sum);
	}
	return;
}

void Forward(int length, double *inputs)
{
	// TODO
	// give the inputs to the first layer
	Sum(length, inputs, firstLayer);
	// while the next layer is not NULL, do the same ?
	for (Layer l = firstLayer.next; l != NULL; l = l.next)
	{
		// get the list of outputs ?
		// or maybe do a separate function that does the same as Sum
		// anyway : PROPAGATE
	}
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

void Backward(Network net, TrainingData data, int run)
{
	// TODO
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
    (void)data;
    (void)net;
    (void)run;
}

void Result(Network net, TrainingData data, int nbrun)
{
	for (int run = 0; run < nbrun; run++)
	{
		double r = net.outputs[run][0];
		char res = 0;
		for (char i = 1; i < 26; i++)
		{
			double rr = GetMax(r, net.outputs[run][(int)i]);
			if (r != rr)
				res = i;
			r = rr;
		}
		char *got = "Predicted Output =";
		char *want = "Expected Output =";
		res += 'A';
		printf("Run %i : %s %c | %s %c\n",
			run, got, res, want, data.expected[run]);
	}
}

void train(int nbrun, Network net, TrainingData data)
{
	// learning rate
	data.lr = 0.1f;

	for (int run = 0; run < nbrun; run++)
	{
		for (int i = 0; i < numTrainingSets; i++)
		{
		    Forward(data.size, data.inputs[run]);
		    Backward(net, data, run);
		}
	}
	Result(net, data, nbrun);
}

int main()
{
	// Init of Training Data


	// Init of Network
	Network network = CreateNet();
	DestroyNet(network);

	// Init of weights
	// --> done in CreateLayer() ?
	init_weights(numInputs, numHiddenNodes,
		    hiddenWeights);
	init_weights(numHiddenNodes, numOutputs,
		    outputWeights);
	// Init of bias
	// --> done in CreateLayer() ?
	init_biases(numOutputs, outputLayerBias);

	int nbrun = 100000;

	train(nbrun, network, data);

	return 0;
}
