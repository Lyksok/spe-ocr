#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "neural.h"
#include "setup.h"
#include "structures.h"

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

void Forward(int length, Layer l, Network net)
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
		net.outputs[n] = l.neurons[n].value;
	}
	// then we transform it to probabilities
	Softmax(l.numNeurons, &net.outputs);
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

void Backward(Network net, TrainingData data, int run)
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
		double r = (data.expected[run] - 'A' == n) ? 1 : 0;
		errors[n] += Cost(data.outputs[run][n], r);
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
		Update(l[0], errors, data.lr);
		free(errors);
		errors = err;
	}
	free(errors);
}

void Result(TrainingData data, int nbrun)
{
	int succ = 0;
	for (int run = 0; run < nbrun; run++)
	{
		double r = data.outputs[run][0];
		char res = 0;
		for (char i = 1; i < 26; i++)
		{
			double rr = GetMax(r, data.outputs[run][(int)i]);
			if (r != rr)
				res = i;
			r = rr;
		}
		char *got = "Predicted Output =";
		char *want = "Expected Output =";
		res += 'A';
		char eres = data.expected[run];
		printf("Run %i : %s %c | %s %c\n",
			run, got, res, want, eres);
		if (res == eres)
			succ += 1;
	}
	double acc = (double) succ/nbrun;
	printf("Accuracy for this set : %f\n", acc);
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
	// learning rate
	data.lr = 0.1f;

	for (int run = 0; run < nbrun; run++)
	{
		for (int i = 0; i < data.nbinputs; i++)
		{
			int len = data.size;
			Copy(len, data.inputs[run], &(net.layers.inputs));
			Forward(len, net.layers, net);
			Copy(26, net.outputs, &(data.outputs[i]));
			Backward(net, data, run);
		}
	}
	Result(data, nbrun);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (argc < 2)
	{
		printf("Expected more arguments\n");
		return 1;
	}
	if (!(strcmp(argv[1], "train")))
	{
		// we train
		printf("Training new network\n");
		// Init of Training Data
		TrainingData data = CreateData(dimension, nTrainingSets);
		// TODO
		// actually create the data

		// Init of Network
		Network network = CreateNet(nLayers, data.size);

		int nbrun = 100000;
		Train(nbrun, network, data);

		DestroyData(data);
		PrintData(network);
		DestroyNet(network);
		return 0;
	}
	else
	{
		// we want an actual answer
		printf("Using current network\n");
		Network network = RecoverNet();
		DestroyNet(network);
		return 0;
	}

	return 0;
}
