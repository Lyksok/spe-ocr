#include <stddef.h>
#pragma once

double sigmoid(double x);

double dSigmoid(double x);

double init_weights();

void shuffle(int *array, size_t n);
