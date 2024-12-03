#ifndef SETUP_H
#define SETUP_H

#include <stddef.h>

#include "structures.h"

double GetMax(double x, double y);
double RandFrom(double min, double max);
void InitWeigths(Layer layer);
void InitBiases(Layer layer);
void PrintData(Network net);
Layer CreateFirstLayer(int len, int nn);
Layer CreateLayer(Layer *l, int nn);
void DestroyLayer(Layer layer);
TrainingData CreateData(int dim, int ni);
void DestroyData(TrainingData data);
Network CreateNet(int numLayers, int len);
void DestroyNet(Network network);

#endif
