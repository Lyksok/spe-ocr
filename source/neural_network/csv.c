#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setup.h"
#include "structures.h"

#define MAX_FIELDS nInputs
#define MAX_FIELD_LEN 7
// because we store each value on 6 char + ','
// also 6 char + '\0' for parsing
#define MAX_LINE_LEN (MAX_FIELD_LEN * MAX_FIELDS + 50)
// the 50 is there for safety

void WriteCsvWeight(const char *filename, Layer l)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
	printf("Unable to open file for writing\n");
        return;
    }

    for (int n = 0; n < l.numNeurons; n++)
    {
	    for (int w = 0; w < l.numWeights - 1; w++)
	    {
		    fprintf("%.6g,", l.weights[n][w]);
	    }
	    fprintf("%.6g\n", l.weights[n][l.numWeights - 1]);
    }
    fclose(file);
}

void WriteCsvBiases(const char *filename, Layer l)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
	printf("Unable to open file for writing\n");
        return;
    }

    for (int n = 0; n < l.numNeurons; n++)
    {
	    fprintf("%.6g,", l.neurons[n].bias);
    }
    fprintf("%.6g\n", l.neurons[l.numNeurons - 1].bias);
    fclose(file);
}

//----- TO CHECK -----//

int ParseLine(const char *line,
		int max_fields,
		char fields[max_fields][MAX_FIELD_LEN])
{
    int field_count = 0;
    char temp_line[MAX_FIELD_LEN];
    strncpy(temp_line, line, MAX_FIELD_LEN);

    char *token = strtok(temp_line, ",");
    while (token != NULL && field_count < max_fields) {
        strncpy(fields[field_count], token, MAX_FIELD_LEN);
        field_count++;
        token = strtok(NULL, ",");
    }

    return field_count;
}

//----- TO CHECK -----//

void ReadCsvWeigths(const char *filename, Layer *layer, int max_weights)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		perror("Unable to open file for reading");
		return;
	}

	char line[MAX_LINE_LENGTH];
	int nodes = 0;
	layer->numWeights = max_weights;
	int max_neurons = layer->numNeurons;
	// we assume ReadCsvBiases is called before ReadCsvWeigths

	// we assume every line is correct
	while (fgets(line, sizeof(line), file) && nodes < max_neurons)
	{
		char fields[max_weights][MAX_FIELD_LEN];
		// TODO: check fields' use in functions
		int num_fields = ParseLine(line, max_weights, fields);
		// should be equal to maw_weigths

		for (int w = 0; w < num_fields; w++)
		{
			layer->weights[nodes][w] = atof(fields[w]);
		}
		nodes++;
	}

	fclose(file);
}

void ReadCsvBiases(const char *filename, Layer *layer, int max_neurons)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		perror("Unable to open file for reading");
		return;
	}

	char line[MAX_LINE_LENGTH];
	int nodes = 0;
	layer->numNeurons = max_neurons;
	layer->neurons = malloc(max_neurons * sizeof(Neuron));

	// we assume every line is correct
	while (fgets(line, sizeof(line), file) && nodes < max_neurons)
	{
		// should only have one bias per line
		char fields[1][MAX_FIELD_LEN];
		// TODO: check fields' use in functions
		int num_fields = ParseLine(line, 1, fields);
		// should be equal to max_neurons

		layer->neurons[nodes].bias = atof(fields[0]);
		nodes++;
	}

	fclose(file);
}
