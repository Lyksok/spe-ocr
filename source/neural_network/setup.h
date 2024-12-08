#ifndef SETUP_H
#define SETUP_H

#include <stddef.h>

#include "convertion.h"
#include "neural_structures.h"

//extern const int dimension;

#define nLayers 2
#define nInputs (dimension * dimension)
#define nNodes 50
#define nOut 26

double GetMax(double x, double y);
double RandFrom(double min, double max);
void InitWeigths(Layer *layer);
void InitBiases(Layer *layer);
void RecoverWeigths(Layer l, int rows, int cols, double mat[rows][cols]);
void RecoverBiases(Layer l, int nodes, double arr[nodes]);
void PrintData(Network net);
Neuron *CreateNeuron();
void DestroyNeuron(Neuron *n);
Layer *CreateFirstLayer(int ni, int nn);
Layer *CreateLayer(Layer *l, int nn);
void SaveLayers(Layer *first);
Layer *RecoverFirstLayer(const char *fweight, const char *fbias);
void RecoverSecondLayer(Layer *l, const char *fweight, const char *fbias);
void DestroyLayer(Layer *layer);
TrainingData *CreateData();
TrainingData *LinkData(TrainingData *d1, TrainingData *d2);
void DestroyData(TrainingData* data);
Network *CreateNet(int numLayers, int lr);
Network *RecoverNet(const char *fw1, const char *fb1,
		const char *fw2, const char *fb2);
void DestroyNet(Network *network);

#endif
