#ifndef CSV_H
#define CSV_H

#include "neural_structures.h"
#include "setup.h"

#define MAX_FIELDS nInputs
#define MAX_FIELD_LEN 7
// because we store each value on 6 char + ','
// also 6 char + '\0' for parsing
#define MAX_LINE_LEN (MAX_FIELD_LEN * MAX_FIELDS + 50)
// the 50 is there for safety

void WriteCsvWeight(const char *filename, Layer l);
void WriteCsvBiases(const char *filename, Layer l);
int ParseLine(const char *line, int max_fields,
              char fields[max_fields][MAX_FIELD_LEN]);
void ReadCsvWeigths(const char *filename, Layer *layer, int max_weights);
void ReadCsvBiases(const char *filename, Layer *layer, int max_neurons);

#endif /* CSV_H */
