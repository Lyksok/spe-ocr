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

void InitWeigths(Layer *layer)
{
	for (int n = 0; n < layer->numNeurons ; n++)
	for (int w = 0; w < layer->numWeights ; w++)
	{
		layer->weights[n][w] = RandFrom(-1, 1);
	}
	return;
}

void InitBiases(Layer *layer)
{
	for (int n = 0; n < layer->numNeurons ; n++)
	{
		layer->neurons[n]->bias = 0;
	}
	return;
}

void PrintData(Network net) {
	Layer *l = net.layers;
	int nl = 0;
	for (; l->next != NULL; l = l->next)
	{
		nl++;
		printf("Weigths of layer %i :\n", nl);
		for (int i = 0; i < l->numWeights; i++)
		{
			for (int n = 0; n < l->numNeurons; n++)
			{
				// each weigths
				printf("%f ", l->weights[i][n]);
			}
			printf("\n");
		}
		printf("Biases of layer %i :\n", nl);
		for (int n = 0; n < l->numNeurons; n++)
		{
			// each biases
			printf("%f ", l->neurons[n]->bias);
		}
		printf("\n");
	}
	return;
}

Neuron *CreateNeuron()
{
	Neuron *n = malloc(sizeof(Neuron));
	n->bias = 0;
	n->value = 0;
	return n;
}

void DestroyNeuron(Neuron *n)
{
	free(n);
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

Layer *RecoverFirstLayer(const char *fweight, const char *fbias)
{
        Layer *layer = malloc(sizeof(Layer));
        layer->numNeurons = nNodes;
        layer->neurons = malloc(nNodes * sizeof(Neuron*));
        layer->inputs = calloc(nInputs, sizeof(double));
        layer->numWeights = nInputs;
	layer->weights = malloc(layer->numNeurons * sizeof(double*));
	for (int n = 0; n < layer->numNeurons; n++)
	{
		layer->neurons[n] = CreateNeuron();
		layer->weights[n] = calloc(layer->numWeights, sizeof(double));
	}
        ReadCsvBiases(fbias, layer, layer->numNeurons);
        ReadCsvWeigths(fweight, layer, layer->numWeights);
        layer->prev = NULL;
        layer->next = NULL;
        return layer;
}

void RecoverSecondLayer(Layer *l, const char *fweight, const char *fbias)
{
        Layer *layer = malloc(sizeof(Layer));
        layer->numNeurons = nOut;
        layer->numWeights = nNodes;
        layer->neurons = malloc(nOut * sizeof(Neuron*));
        layer->inputs = calloc(nNodes, sizeof(double));
	layer->weights = malloc(layer->numNeurons * sizeof(double*));
	for (int n = 0; n < layer->numNeurons; n++)
	{
		layer->neurons[n] = CreateNeuron();
		layer->weights[n] = calloc(layer->numWeights, sizeof(double));
	}
        ReadCsvBiases(fbias, layer, layer->numNeurons);
        ReadCsvWeigths(fweight, layer, layer->numWeights);

        layer->prev = l;
        l->next = layer;
        layer->next = NULL;
        return;
}

Layer *CreateFirstLayer(int ni, int nn)
{
        Layer *layer = malloc(sizeof(Layer));
	layer->numNeurons = nn;
	layer->numWeights = ni;
        layer->neurons = malloc(nn * sizeof(Neuron*));
	layer->inputs = calloc(ni, sizeof(double));
	layer->weights = malloc(layer->numNeurons * sizeof(double*));
	for (int n = 0; n < layer->numNeurons; n++)
	{
		layer->neurons[n] = CreateNeuron();
		layer->weights[n] = calloc(layer->numWeights, sizeof(double));
	}
//	InitBiases(layer);
	InitWeigths(layer);
	layer->prev = NULL;
	layer->next = NULL;
        return layer;
}

Layer *CreateLayer(Layer *l, int nn)
{
	int ni = l->numNeurons;
        Layer *layer = CreateFirstLayer(ni, nn);
	layer->prev = l;
	l->next = layer;
	layer->next = NULL;
        return layer;
}

void DestroyLayer(Layer *layer)
{
	if (layer == NULL)
	{
		return;
	}
	else
	{
		for (int n = 0; n < layer->numNeurons; n++)
		{
			DestroyNeuron(layer->neurons[n]);
			free(layer->weights[n]);
		}
		free(layer->weights);
		free(layer->neurons);
		free(layer->inputs);
		DestroyLayer(layer->next);
		free(layer);
		return;
	}
}

TrainingData *CreateData()
{
	TrainingData *data = malloc(sizeof(TrainingData));
	data->inputs = calloc(nInputs, sizeof(double));
	data->expected = 0;
	data->next = NULL;
	return data;
}

TrainingData *LinkData(TrainingData *d1, TrainingData *d2)
{
	if (d1 == NULL)
	{
		d1 = d2;
		return d1;
	}
	else
	{
		d1->next = d2;
		return d1;
	}
}

void DestroyData(TrainingData *data)
{
	if (data == NULL)
	{
		return;
	}
	else
	{
		free(data->inputs);
		DestroyData(data->next);
		free(data);
		return;
	}
}

Network *CreateNet(int numLayers, int lr)
{
        Network *network = malloc(sizeof(Network));

	Layer *l = CreateFirstLayer(nInputs, nNodes);
	network->layers = l;
        for (int i = 1; i < numLayers; i++)
        {
		l = CreateLayer(l, nOut);
        }

	network->lr = lr;

        return network;
}

Network *RecoverNet(const char *fw1, const char *fb1,
                const char *fw2, const char *fb2)
{
        Network *net = malloc(sizeof(Network));
        net->layers = RecoverFirstLayer(fw1, fb1);
	RecoverSecondLayer(net->layers, fw2, fb2);
	net->lr = 0.1;
        return net;
}

void DestroyNet(Network *network)
{
	DestroyLayer(network->layers);
	free(network);
        return;
}
