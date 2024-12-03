#include <stdlib.h>
#include <stdio.h>

#include "structures.h"

double GetMax(double x, double y)
{
	return x > y ? x : y;
}

double RandFrom(double min, double max)
{
        double range = max - min;
        double div = RAND_MAX / range;
        return min + (rand() / div);
}

void InitWeigths(Layer layer)
{
	for (int n = 0; n < layer.numNeurons ; n++)
	for (int w = 0; w < layer.numWeights ; w++)
	{
		layer.weights[n][w] = RandFrom(-1, 1);
	}
	return;
}

void InitBiases(Layer layer)
{
	for (int n = 0; n < layer.numNeurons ; n++)
	{
		layer.neurons[n].bias = 0;
	}
	return;
}

void PrintData(Network net) {
	Layer l = net.layers;
	int nl = 0;
	for (; l.next != NULL; l = l.next[0])
	{
		nl++;
		printf("Weigths of layer %i :\n", nl);
		for (int i = 0; i < l.numWeights; i++)
		{
			for (int n = 0; n < l.numNeurons; n++)
			{
				// each weigths
				printf("%f ", l.weights[i][n]);
			}
			printf("\n");
		}
		printf("Biases of layer %i :\n", nl);
		for (int n = 0; n < l.numNeurons; n++)
		{
			// each biases
			printf("%f ", l.neurons[n].bias);
		}
		printf("\n");
	}
	return;
}

Layer CreateFirstLayer(int len, int nn)
{
        Layer layer;
	layer.numNeurons = nn;
        layer.neurons = malloc(nn * sizeof(Neuron));
	layer.inputs = calloc(len, sizeof(double));
	InitBiases(layer);
	layer.numWeights = len;
	InitWeigths(layer);
	layer.prev = NULL;
	layer.next = NULL;
        return layer;
}

Layer *CreateLayer(Layer *l, int nn)
{
	int len = l->numNeurons;
        Layer temp = CreateFirstLayer(len, nn);
	Layer *layer = &temp;
	layer->prev = l;
	l->next = layer;
	layer->next = NULL;
        return layer;
}

void DestroyLayer(Layer layer)
{
	free(layer.neurons);
	free(layer.inputs);
	if (layer.next == NULL)	
		return;
	else
	{
		DestroyLayer(layer.next[0]);
	}
}

TrainingData CreateData(int dim, int ni)
{
	// TODO
	TrainingData data;
	// the size of the list
	// that is the dimensions of the image
	// eg : if image is of 13x13, then size = 13 * 13
	data.size = dim * dim;
	// the number of inputs we have
	data.nbinputs = ni;
	// the list of inputs
	data.inputs = malloc(ni * sizeof(double*));
	for (int i = 0; i < ni; i++)
	{
		// each input
		// one input = a list of 0 and 1
		data.inputs[i] = calloc(data.size, sizeof(double));
	}
	// what is the expected result (as a character)
	data.expected = calloc(ni, sizeof(char));
	return data;
}

void DestroyData(TrainingData data)
{
	// TODO
	for (int i = 0; i < data.nbinputs; i++)
	{
		free(data.inputs[i]);
	}
	free(data.inputs);
	free(data.expected);
	return;
}

Network CreateNet(int numLayers, int len)
{
        Network network;
	network.nbsuccess = 0;
	network.nbruns = 0;
	// nn : number of neurons inside the layers
	int nn = 0;
	network.layers = CreateFirstLayer(len, nn);
	Layer *l = &(network.layers);
        for (int i = 1; i < numLayers; i++)
        {
		l = CreateLayer(l, nn);
        }

        return network;
}

void DestroyNet(Network network)
{
	DestroyLayer(network.layers);
        return;
}
