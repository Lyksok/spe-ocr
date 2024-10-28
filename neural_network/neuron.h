#pragma once

typedef struct {
	double *weights;
	double bias;
} Neuron;

void initialize_neuron(Neuron *neuron);
void backward(Neuron *neuron, double *inputs, double target);
