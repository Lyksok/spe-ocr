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
		double sum = 0;
		// foreach inputs
		for (int i = 0; i < length; i++)
		{
			double w = layer.weights[n][i];
			// we add the value times the weight
			sum += inputs[i] * w;
		}
		// then we add the bias
		sum += neuron.bias;
		// and apply the activation function
		neuron.value = sigmoid(sum);
	}
	return;
}

void SumHidden(Layer l1, Layer l2)
{
	double *inputs;
	int length = l1.numNeurons;
	// foreach output
	for (int n = 0; n < length; n++)
	{
		inputs[n] = l1.neurons[n].value;
	}
	// then we call Sum with the "inputs"
	Sum(length, inputs, l2);
	return;
}

void Forward(int length, double *inputs, Layer l, double *outputs)
{
	// give the inputs to the first layer
	Sum(length, inputs, l);
	// while the next layer is not NULL
	for (; l.next != NULL; l = l.next[0])
	{
		// PROPAGATE
		SumHidden(l, l.next[0]);
	}

	// we collect the result of the last layer
	for (int n = 0; n < l.numNeurons; n++)
	{
		outputs[n] = l.neurons[n].value;
	}
	// then we transform it to probabilities
	Softmax(l.numNeurons, outputs);
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
		    Forward(data.size, data.inputs[run],
			net.layers[0], net.outputs[run]);
		    Backward(net, data, run);
		}
	}
	Result(net, data, nbrun);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	// Init of Training Data
	TrainingData data = CreateData();
	DestroyData(data);

	// Init of Network
	Network network = CreateNet();
	DestroyNet(network);

	// Init of weights
	// --> done in CreateLayer() ?
	// weights of input are in the first hidden layer
	// weigths of output tho are not, may need to do that



	// Init of bias
	// --> done in CreateLayer() ?
	// biases of layer are created
	// but not of outputs
	// but ouputs dont need biases ??

	int nbrun = 100000;
	train(nbrun, network, data);

	return 0;
}
