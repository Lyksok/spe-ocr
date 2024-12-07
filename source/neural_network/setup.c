#include <stdlib.h>
#include <stdio.h>

#include "convertion.h"
#include "csv.h"
#include "neural_structures.h"
#include "setup.h"

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

void SaveLayers(Layer *first)
{
        const char *fbias_1 = "network/fbias_1.csv";
        const char *fweight_1 = "network/fweight_1.csv";
        WriteCsvBiases(fbias_1, *first);
        WriteCsvWeight(fweight_1, *first);

        const char *fbias_2 = "network/fbias_2.csv";
        const char *fweight_2 = "network/fweight_2.csv";
        WriteCsvBiases(fbias_2, *(first->next));
        WriteCsvWeight(fweight_2, *(first->next));
        return;
}

Layer RecoverFirstLayer(const char *fweight, const char *fbias)
{
        Layer layer;
        layer.numNeurons = nNodes;
        layer.neurons = malloc(nNodes * sizeof(Neuron));
        layer.inputs = calloc(nInputs, sizeof(double));
        layer.numWeights = nInputs;
        ReadCsvBiases(fbias, &layer, layer.numNeurons);
        ReadCsvWeigths(fweight, &layer, layer.numWeights);
        layer.prev = NULL;
        layer.next = NULL;
        return layer;
}

void RecoverSecondLayer(Layer *l, const char *fweight, const char *fbias)
{
        Layer layer;
        layer.numNeurons = nOut;
        layer.neurons = malloc(nOut * sizeof(Neuron));
        layer.inputs = calloc(nNodes, sizeof(double));
        layer.numWeights = nNodes;
        ReadCsvBiases(fbias, &layer, layer.numNeurons);
        ReadCsvWeigths(fweight, &layer, layer.numWeights);

        layer.prev = l;
        l->next = &layer;
        layer.next = NULL;
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

TrainingData CreateData()
{
	TrainingData data;
	data.inputs = calloc(nInputs, sizeof(double));
	// what is the expected result (as a character)
	data.expected = 0;
	data.next = NULL;
	return data;
}

void LinkData(TrainingData *d1, TrainingData *d2)
{
	if (d1 == NULL)
		d1 = d2;
	else
		d1->next = d2;
	return;
}

void DestroyData(TrainingData data)
{
	free(data.inputs);
	DestroyData(*(data.next));
	return;
}

Network CreateNet(int numLayers, int lr)
{
        Network network;

	network.layers = CreateFirstLayer(nInputs, nNodes);
	Layer *l = &(network.layers);
        for (int i = 1; i < numLayers; i++)
        {
		l = CreateLayer(l, nNodes);
        }

	network.lr = lr;

        return network;
}

Network RecoverNet(const char *fw1, const char *fb1,
                const char *fw2, const char *fb2)
{
        Network net;
        net.layers = RecoverFirstLayer(fw1, fb1);
	RecoverSecondLayer(&(net.layers), fw2, fb2);
        return net;
}

void DestroyNet(Network network)
{
	DestroyLayer(network.layers);
        return;
}
