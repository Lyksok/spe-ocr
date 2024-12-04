#ifndef SETUP_H
#define SETUP_H

#include <stddef.h>

#include "structures.h"

#define nLayers 2
#define nTrainingSets 4
#define nNodes 16
#define nOut 26

extern int dimension;
int nInputs = dimension * dimension;

double HiddenBias[nNodes] = {};
double HiddenWeight[nNodes][nInputs] = {};
double OutputBias[nOut] = {};
double OutputWeight[nOut][nNodes] = {};

double GetMax(double x, double y);
double RandFrom(double min, double max);
void InitWeigths(Layer layer);
void InitBiases(Layer layer);
void RecoverWeigths(Layer l, int rows, int cols, double mat[rows][cols]);
void RecoverBiases(Layer l, int nodes, double arr[nodes]);
void PrintData(Network net);
Layer CreateFirstLayer(int len, int nn);
Layer CreateLayer(Layer *l, int nn);
Layer RecoverFirstLayer();
void RecoverSecondLayer(Layer *l);
void DestroyLayer(Layer layer);
TrainingData CreateData(int dim, int ni);
void DestroyData(TrainingData data);
Network CreateNet(int numLayers, int len);
Network RecoverNet();
void DestroyNet(Network network);

#endif
