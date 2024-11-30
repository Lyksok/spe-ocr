#include <stdlib.h>
#include <stdio.h>

#include "structures.h"

double RandFrom(double min, double max)
{
        double range = max - min;
        double div = RAND_MAX / range;
        return min + (rand() / div);
}

void init_weights(int row, int col, double **array)
{

        for(int i = 0; i < row; i++)
        {
                for(int j = 0; j < col; j++)
                {
                        array[i][j] = RandFrom(-1, 1);
                }
        }
}

void init_biases(int length, double *array)
{
        for(int i = 0; i < length; i++)
        {
                array[i] = 0;
        }
}

Layer CreateLayer()
{
        // TODO
        Layer layer;
        layer.nodesIn = 0;
        layer.nodesOut = 0;
        layer.neurons = malloc(0);
        return layer;
}

void DestroyLayer(Layer layer)
{
        // TODO
        free(layer.neurons);
        return;
}

Network CreateNet()
{
        // TODO
        Network network;
	network.numLayers = 0;
        network.layers = malloc(numLayers);
        for (int i = 0; i < network.numLayers; i++)
        {
                network.layers[i] = CreateLayer();
        }

        return network;
}

void DestroyNet(Network network)
{
        // TODO
	for (int i = 0; i < network.numLayers; i++)
	{
		DestroyLayer(network.layers[i]);
	}
        free(network.layers);
        return;
}
