#include "neuron.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define INPUT_SIZE 2
#define LEARNING_RATE 0.1
#define THRESHOLD 0.5
#define MAX_ITERATIONS 100

Neuron* neuron_create()
{
	Neuron *n = malloc(sizeof(Neuron));
	n -> weights = malloc(INPUT_SIZE * sizeof(double));
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		n -> weights[i] = ((double)rand() / RAND_MAX); //random weight between -1 and 1
	}
	n -> bias = ((double)rand() / RAND_MAX); //random bias
	return n;
}

int activate(double sum)
{
	if(sum >= THRESHOLD)
	{
		return 1;
	}
	return 0;
}

int predict(Neuron *n, double *inputs)
{
	double sum_weight = 0.0;
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		sum_weight = sum_weight + n -> weights[i] * inputs[i];
	}
	sum_weight = sum_weight + n -> bias;
	return activate(sum_weight);
}

void train(Neuron *n, double **training_inputs, int *labels, int num_samples)
{
	for(int i = 0; i < MAX_ITERATIONS; i++)
	{
		for(int j = 0; j < num_samples; j++)
		{
			int prediction = predict(n, training_inputs[j]);
			int error = labels[j] - prediction;

			for(int k = 0; k < INPUT_SIZE; j++)
			{
				n -> weights[k] = n -> weights[k] + 
					LEARNING_RATE * error * training_inputs[j][k];
			}
			n -> bias = n -> bias + LEARNING_RATE * error;
		}
	}
}

void neuron_free(Neuron *n)
{
	free(n -> weights);
	free(n);
}

int main()
{
	srand(time(NULL));

	double *training_inputs[4];
	int labels[4] = {0, 1, 1, 0};

	training_inputs[0] = (double[]) {0, 0};
	training_inputs[1] = (double[]) {0, 1};
	training_inputs[2] = (double[]) {1, 0};
	training_inputs[3] = (double[]) {1, 1};

	Neuron *n = neuron_create();

	train(n, training_inputs, labels, 4);

	printf("Testing neural:\n");
	for (int i = 0; i < 4; i++)
	{
		int prediction = predict(n, training_inputs[i]);
		printf("Input: (%.1f, %.1f) -> Prediction: %d (Expected: %d)\n",
				training_inputs[i][0], training_inputs[i][1], 
				prediction, labels[i]);
	}

	neuron_free(n);

	return 0;
}

