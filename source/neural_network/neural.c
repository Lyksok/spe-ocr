#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "neural.h"
#include "neural_structures.h"
#include "read_data.h"
#include "setup.h"

void Softmax(int length, double **mat)
{
        double sum = 0;
        for (int n = 0; n < length; n++)
        {
		*mat[n] = exp(*mat[n]);
                sum += *mat[n];
        }
        for (int n = 0; n < length; n++)
        {
                *mat[n] = (*mat[n]) / sum;
        }
}

double sigmoid(double x)
{
        return 1 / (1 + exp(-x));
}

double dSigmoid(double x)
{
        return x * (1 - x);
}

void Sum(int length, Layer layer)
{
	double *inputs = layer.inputs;
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
	int length = l1.numNeurons;
	for (int n = 0; n < length; n++)
	{
		l2.inputs[n] = l1.neurons[n].value;
	}
	Sum(length, l2);
	return;
}

void Forward(int length, Layer l, Network net, int i)
{
	Sum(length, l);
	// while the next layer is not NULL
	for (; l.next != NULL; l = l.next[0])
	{
		// PROPAGATE
		SumHidden(l, l.next[0]);
	}

	// we collect the result of the last layer
	for (int n = 0; n < l.numNeurons; n++)
	{
		net.outputs[i][n] = l.neurons[n].value;
	}
	// then we transform it to probabilities
	Softmax(l.numNeurons, &(net.outputs[i]));
}

void Update(Layer l, double *errors, int lr) {
	for (int n = 0; n < l.numNeurons; n++)
	{
		double e = errors[n];
		for (int i = 0; i < l.numWeights; i++)
		{
			double w = l.weights[n][i];
			double in = l.inputs[i];
			l.weights[n][i] = w - lr * e * in;
		}
		double b = l.neurons[n].bias;
		l.neurons[n].bias = b - lr * e;
	}
	return;
}

double Cost(double a, double b) {
	double c = a - b;
	return c * c;
}

double dCost(double a, double b) {
	double c = a - b;
	return 2 * c;
}

void Backward(Network net, TrainingData data, int i)
{
	Layer *l = &(net.layers);
	for (; l->next != NULL; l = l->next)
	{}

	double *errors = calloc(26, sizeof(double));
	if (errors == NULL)
	{
		printf("Backward() -> Calloc()\n");
		return;
	}
	for (int n = 0; n < 26; n++)
	{
		double r = (data.expected - 'A' == n) ? 1 : 0;
		errors[n] += Cost(net.outputs[i][n], r);
	}
	for (; l != NULL; l = l->prev)
	{
		int nb = l->numNeurons;
		double *err = calloc(nb, sizeof(double));
		if (err == NULL)
		{
			printf("Backward() -> Calloc()\n");
			return;
		}
		for (int n = 0; n < nb; n++)
		{
			double nerr = 0;
			for (int x = 0; x < l->numWeights; x++)
			{
				double w = l->weights[x][n];
				nerr += err[n] * w;
			}
			double v = l->neurons[n].value;
			nerr *= dSigmoid(v);
			err[n] += nerr;
		}
		Update(l[0], errors, net.lr);
		free(errors);
		errors = err;
	}
	free(errors);
}

double Result(TrainingData data, Network net, int run)
{
	int succ = 0;
	TrainingData *cur = &data;
	int i = 0;
	for (; cur != NULL; cur = cur->next)
	{
		double r = net.outputs[i][0];
		char res = 0;
		for (char c = 1; c < 26; c++)
		{
			double rr = GetMax(r, net.outputs[i][(int)c]);
			if (r != rr)
				res = c;
			r = rr;
		}
		char *got = "Predicted Output =";
		char *want = "Expected Output =";
		res += 'A';
		char eres = data.expected;
		printf("Run %i : %s %c | %s %c\n",
			run, got, res, want, eres);
		if (res == eres)
			succ += 1;
		i++;
	}
	double acc = (i != 0) ? (double) succ/i : 0;
	printf("Accuracy for this set : %f\n", acc);
	return acc;
}

void Copy(int len, double *src, double **dest) {
	for (int l = 0; l < len; l++)
	{
		*dest[l] = src[l];
	}
	return;
}

void Train(int nbrun, Network net, TrainingData data)
{
	double accuracy = 0;
	for (int run = 0; run < nbrun; run++)
	{
		for (int i = 0; i < nInputs; i++)
		{
			int len = dimension * dimension;
			Copy(len, data.inputs, &(net.layers.inputs));
			Forward(len, net.layers, net, i);
			//Copy(26, net.outputs, &(net.outputs[i]));
			Backward(net, data, run);
			accuracy += Result(data, net, run);
		}
	}
	printf("Total accuracy : %f\n", (nbrun != 0) ? (accuracy/nbrun) : 0);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (argc < 3)
	{
		printf("Expected more arguments\n");
		return 1;
	}
	if (!(strcmp(argv[1], "train")))
	{
		// we train
		printf("Training new network\n");
		// Init of Training Data
		TrainingData *data = ParseDirectory();

		// Init of Network
		Network network = CreateNet(nLayers, 0.5);

		int nbrun = atoi(argv[2]);
		Train(nbrun, network, *data);

		DestroyData(*data);
		PrintData(network);
		DestroyNet(network);
		return 0;
	}
	else if (!(strcmp(argv[1], "recover")))
	{
		// we want an actual answer
		printf("Using current network\n");
		const char *fb1 = "network/fbias_1.csv";
		const char *fw1 = "network/fweight_1.csv";
		const char *fb2 = "network/fbias_2.csv";
		const char *fw2 = "network/fweight_2.csv";
		Network network = RecoverNet(fw1, fb1, fw2, fb2);
		// argv[2] is a path to the image to read
		// convert to sdl, resize and tranfsorm to list
		// call network and solve
		DestroyNet(network);
		return 0;
	}

	return 0;
}
