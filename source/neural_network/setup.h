#ifndef SETUP_H
#define SETUP_H

#include <stddef.h>

#include "structures.h"

double RandFrom(double min, double max);
void init_weights(int row, int col, double **array);
void init_biases(int length, double *array);
Layer CreateLayer();
void DestroyLayer(Layer layer);
Network CreateNet();
void DestroyNet(Network network);

#endif
