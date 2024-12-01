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
	/*
        layer.numNodesIn = 0;
        layer.nodesIn = malloc(numNodesIn * sizeof(Neuron));
        layer.numNodesOut = 0;
        layer.nodesOut = malloc(numNodesOut * sizeof(Neuron));
	*/
	layer.numNeurons = 0;
        layer.neurons = malloc(numNeurons * sizeof(Neuron));
	InitBiases(layer);
	layer.numWeights = 0;
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

Network CreateNet()
{
        // TODO
        Network network;
	network.numLayers = 0;
        network.layers = malloc(numLayers * sizeof(Layer));
	if (network.numLayers)
	{
		network.layers[0] = CreateLayer();
	}
        for (int i = 1; i < network.numLayers; i++)
        {
                network.layers[i] = CreateLayer();
		network.layers[i - 1].next = network.layers[i];
		network.layers[i].prev = network.layers[i - 1];
        }
	//network.outputs = calloc(26 * sizeof(double));

        return network;
}

void DestroyNet(Network network)
{
        // TODO
	DestroyLayer(network.layers);
	//free(network.outputs);
        free(network.layers);
        return;
}
