#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "neural.h"
#include "neural_structures.h"
#include "read_data.h"
#include "setup.h"

void Softmax(Network *net, int i) {
  double sum = 0;
  int len = nOut;
  for (int n = 0; n < len; n++) {
    double o = net->outputs[i][n];
    net->outputs[i][n] = exp(o);
    sum += net->outputs[i][n];
  }
  for (int n = 0; n < len; n++) {
    double o = net->outputs[i][n];
    net->outputs[i][n] = o / sum;
  }
}

double sigmoid(double x) { return 1 / (1 + exp(-x)); }

double dSigmoid(double x) { return x * (1 - x); }

void Sum(int length, Layer *layer) {
  double *inputs = layer->inputs;
  // foreach neurons
  for (int n = 0; n < layer->numNeurons; n++) {
    //		printf("We are at neuron number %i\n", n + 1);
    Neuron *neuron = layer->neurons[n];
    // we initialize the value of the neuron
    double sum = 0;
    // foreach inputs
    for (int i = 0; i < length; i++) {
      double w = layer->weights[n][i];
      // we add the value times the weight
      sum += inputs[i] * w;
    }
    // then we add the bias
    sum += neuron->bias;
    // and apply the activation function
    neuron->value = sigmoid(sum);
    layer->neurons[n] = neuron;
  }
  return;
}

void SumHidden(Layer *l1, Layer *l2) {
  int length = l1->numNeurons;
  for (int n = 0; n < length; n++) {
    l2->inputs[n] = l1->neurons[n]->value;
  }
  Sum(length, l2);
  return;
}

void Forward(int length, Network *net, int i) {
  Layer *l = net->layers;
  Sum(length, l);
  // while the next layer is not NULL
  for (; l->next != NULL; l = l->next) {
    // PROPAGATE
    //		printf("propagating\n");
    SumHidden(l, l->next);
  }
  //	if (l->next == NULL) printf("next is NULL\n");
  //	else l = l->next;

  //	printf("there are %i neurons in this layer\n", l->numNeurons);
  // we collect the result of the last layer
  for (int n = 0; n < l->numNeurons; n++) {
    //		printf("collecting from neuron number %i\n", n);
    net->outputs[i][n] = l->neurons[n]->value;
  }
  // then we transform it to probabilities
  Softmax(net, i);
}

double Cost(double a, double b) {
  double c = a - b;
  return c * c;
}

double dCost(double a, double b) {
  double c = a - b;
  return 2 * c;
}

double *GetError(Layer *l) {
  int nb = l->numNeurons;
  //	printf("getting error and calloc of %i\n", nb);
  double *err = calloc(nb, sizeof(double));
  if (err == NULL) {
    printf("GetError() -> Calloc()\n");
    return NULL;
  }
  for (int n = 0; n < nb; n++) {
    double nerr = 0;
    for (int x = 0; x < l->numWeights; x++) {
      double w = l->weights[n][x];
      nerr += l->inputs[x] * w;
    }
    double v = l->neurons[n]->value;
    nerr *= dSigmoid(v);
    err[n] += nerr;
  }
  return err;
}

void Update(Layer *l, double *errors, int lr) {
  for (int n = 0; n < l->numNeurons; n++) {
    //		printf("updating neuron %i\n", n);
    double e = errors[n];
    for (int i = 0; i < l->numWeights; i++) {
      double w = l->weights[n][i];
      double in = l->inputs[i];
      l->weights[n][i] = w - lr * e * in;
    }
    double b = l->neurons[n]->bias;
    l->neurons[n]->bias = b - lr * e;
  }
  return;
}

void Backward(Network *net, TrainingData data, int i) {
  //	printf("entering Backward()\n");
  Layer *l = net->layers;
  for (; l->next != NULL; l = l->next) {
  }

  double *errors = calloc(26, sizeof(double));
  if (errors == NULL) {
    printf("Backward() -> Calloc()\n");
    return;
  }
  for (int n = 0; n < 26; n++) {
    double r = (data.expected - 'A' == n) ? 1 : 0;
    errors[n] += Cost(net->outputs[i][n], r);
  }
  for (; l->prev != NULL; l = l->prev) {
    //		printf("Updating layer with %d neurons\n", l->numNeurons);
    Update(l, errors, net->lr);
    double *err = GetError(l->prev);
    if (err == NULL) {
      free(errors);
      return;
    }
    free(errors);
    errors = err;
  }
  // updating first layer
  Update(l, errors, net->lr);
  free(errors);
}

double Result(TrainingData *data, Network net, int run) {
  int succ = 0;
  TrainingData *cur = data;
  int i = 0;
  for (; cur != NULL; cur = cur->next) {
    double r = net.outputs[i][0];
    char res = 0;
    for (char c = 1; c < 26; c++) {
      double rr = GetMax(r, net.outputs[i][(int)c]);
      if (r != rr)
        res = c;
      r = rr;
    }
    char *got = "Predicted Output =";
    char *want = "Expected Output =";
    res += 'A';
    char eres = cur->expected;
    printf("Run %i : %s %c | %s %c\n", run, got, res, want, eres);
    if (res == eres)
      succ += 1;
    i++;
  }
  double acc = (i != 0) ? (double)succ / i : 0;
  printf("Accuracy for this set : %f\n", acc);
  return acc;
}

void Copy(int len, double *src, double **dest) {
  for (int l = 0; l < len; l++) {
    *dest[l] = src[l];
  }
  return;
}

void CopyDataToLayer(TrainingData data, Layer *layer) {
  for (int d = 0; d < nInputs; d++) {
    layer->inputs[d] = data.inputs[d];
  }
  return;
}

void Train(int nbrun, Network *net, TrainingData *data) {
  int numTrainingData = 0;
  for (TrainingData *cur = data; cur != NULL; cur = cur->next) {
    numTrainingData++;
  }
  net->outputs = malloc(numTrainingData * sizeof(double *));
  if (net->outputs == NULL) {
    printf("Train() -> Malloc()\n");
    return;
  }
  for (int t = 0; t < numTrainingData; t++) {
    net->outputs[t] = calloc(26, sizeof(double));
    if (net->outputs[t] == NULL) {
      printf("Train() -> Calloc()\n");
      for (int f = 0; f < t; f++)
        free(net->outputs[f]);
      free(net->outputs);
      return;
    }
  }
  double accuracy = 0;
  for (int run = 0; run < nbrun; run++) {
    TrainingData *cur = data;
    int i = 0;
    for (; cur != NULL; cur = cur->next) {
      int len = dimension * dimension;
      CopyDataToLayer(*cur, net->layers);
      Forward(len, net, i);
      Backward(net, *cur, run);
      i++;
    }
    accuracy += Result(data, *net, run);
  }
  for (int t = 0; t < numTrainingData; t++) {
    free(net->outputs[t]);
  }
  free(net->outputs);
  printf("Total accuracy : %f\n", (nbrun != 0) ? (accuracy / nbrun) : 0);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Expected more arguments\n");
    return 1;
  }
  if (!(strcmp(argv[1], "train"))) {
    // we train
    printf("Training new network\n");
    // Init of Training Data
    TrainingData *data = ParseDirectory();

    // Init of Network
    Network *network = CreateNet(nLayers, 0.5);

    int nbrun = atoi(argv[2]);
    Train(nbrun, network, data);

    DestroyData(data);
    //		PrintData(*network);
    DestroyNet(network);
    return 0;
  } else if (!(strcmp(argv[1], "recover"))) {
    // we want an actual answer
    printf("Using current network\n");
    const char *fb1 = "network/fbias_1.csv";
    const char *fw1 = "network/fweight_1.csv";
    const char *fb2 = "network/fbias_2.csv";
    const char *fw2 = "network/fweight_2.csv";
    Network *network = RecoverNet(fw1, fb1, fw2, fb2);
    // argv[2] is a path to the image to read
    // convert to sdl, resize and tranfsorm to list
    // call network and solve
    DestroyNet(network);
    return 0;
  }

  return 0;
}
