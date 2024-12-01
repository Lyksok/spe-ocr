#ifndef SETUP_H
#define SETUP_H

#include <stddef.h>

#include "structures.h"

double RandFrom(double min, double max);
void InitWeigths(Layer layer);
void InitBiases(Layer layer);
Layer CreateLayer();
void DestroyLayer(Layer layer);
Network CreateNet();
void DestroyNet(Network network);

#endif
