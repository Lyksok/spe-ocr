#ifndef NEURAL_LETTER_H
#define NEURAL_LETTER_H

#include <stddef.h>

#include "convertion.h"
#include "neural_structures.h"
#include "setup.h"

void Forward(int length, Network *net, int i);

void Backward(Network *net, TrainingData data, int i);

// void Result(Network net, TrainingData data, int nbrun);
double Result(TrainingData *data, Network net, int run);

void Train(int nbrun, Network *net, TrainingData *data);

void Find(Network *net);

#endif
