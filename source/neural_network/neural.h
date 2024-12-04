#ifndef NEURAL_LETTER_H
#define NEURAL_LETTER_H

#include <stddef.h>

#include "convertion.h"
#include "structures.h"
#include "setup.h"

void Forward(int length, Layer l, Network net);

void Backward(Network net, TrainingData data, int run);

void Result(Network net, TrainingData data, int nbrun);

void Train(int nbrun, Network net, TrainingData data);

#endif
