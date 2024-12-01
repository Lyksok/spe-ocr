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
	for (int l = 0; l < layer.numWeights ; l++)
	{
		layer.weights[l] = RandFrom(-1, 1);
	}
	return;
}

void InitBiases(Layer layer)
{
	for (int l = 0; l < layer.numNeurons ; l++)
	{
		layer.neurons[j].bias = 0;
	}
	return;
}

Layer CreateLayer()
{
        // TODO
        Layer layer;
	// the number of neurons of the layer
	layer.numNeurons = 0;
	// the list of said neurons
        layer.neurons = malloc(numNeurons * sizeof(Neuron));
	// the bias of each neuron
	InitBiases(layer);
	// the number of weights given to the layer
	layer.numWeights = 0;
	// the list of weigth given to the layer
	InitWeigths(layer);
	layer.prev = NULL;
	layer.next = NULL;
        return layer;
}

void DestroyLayer(Layer layer)
{
        // TODO
	if (layer == NULL)
		return;
	else
	{
		free(layer.neurons);
		DestroyLayer(layer.next);
	}
}

TrainingData CreateData()
{
	// TODO
	TrainingData data;
	// the size of the list
	// that is the dimensions of the image
	// eg : if image is of 13x13, then size = 13 * 13
	data.size = 0;
	// the number of inputs we have
	data.nbinputs = 0;
	// the list of inputs
	data.inputs = malloc(nbinputs * sizeof(double*));
	for (int i = 0; i < data.nbinputs; i++)
	{
		// each input
		// one input = a list of 0 and 1
		data.inputs[i] = calloc(size, sizeof(double));
	}
	// what is the expected result (as a character)
	data.expected = calloc(nbinputs * sizeof(char));
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
	return;
}

Network CreateNet()
{
        // TODO
        Network network;
	// the number of LAYERS inside the network
	// at least one layer (>= 1)
	network.numLayers = 1;
	// list of said layers
        network.layers = malloc(numLayers * sizeof(Layer));
	if (network.numLayers)
	{
		// first hidden layer is created
		network.layers[0] = CreateLayer();
	}
        for (int i = 1; i < network.numLayers; i++)
        {
		// each layer is created
		// and linked with the previous one
                network.layers[i] = CreateLayer();
		network.layers[i - 1].next = network.layers[i];
		network.layers[i].prev = network.layers[i - 1];
        }

        return network;
}

void DestroyNet(Network network)
{
        // TODO
	DestroyLayer(network.layers);
        free(network.layers);
        return;
}
