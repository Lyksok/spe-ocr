#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setup.h"
#include "structures.h"

#define MAX_FIELDS nInputs
#define MAX_LINE_LENGTH (7 * MAX_FIELDS + 50)
// because we store each value on 6 char + ','
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

int parse_csv_line(const char *line, char fields[MAX_FIELDS][MAX_LINE_LENGTH], int max_fields) {
    int field_count = 0;
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH);

    char *token = strtok(temp_line, ",");
    while (token != NULL && field_count < max_fields) {
        strncpy(fields[field_count], token, MAX_LINE_LENGTH);
        field_count++;
        token = strtok(NULL, ",");
    }

    return field_count;
}

//----- TO CHECK -----//

void read_csv(const char *filename, Record records[], int *record_count, int max_records) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file for reading");
        return;
    }

    char line[MAX_LINE_LENGTH];
    *record_count = 0;

    // Skip header
    if (fgets(line, sizeof(line), file)) {
        printf("Header: %s", line);
    }

    while (fgets(line, sizeof(line), file) && *record_count < max_records) {
        char fields[MAX_FIELDS][MAX_LINE_LENGTH];
        int num_fields = parse_csv_line(line, fields, MAX_FIELDS);

        if (num_fields >= 3) { // Adjust based on required fields
            strncpy(records[*record_count].name, fields[0], sizeof(records[*record_count].name));
            records[*record_count].age = atoi(fields[1]);
            records[*record_count].score = atof(fields[2]);
            (*record_count)++;
        } else {
            fprintf(stderr, "Invalid line: %s", line);
        }
    }

    fclose(file);
}

//----- TO CHECK -----//

int main() {
    const char *filename = "data.csv";

    // Example of creating records dynamically
    Record records[MAX_FIELDS];
    int record_count;

    // Reading CSV file
    read_csv(filename, records, &record_count, MAX_FIELDS);

    // Displaying the recovered data
    for (int i = 0; i < record_count; i++) {
        printf("Name: %s, Age: %d, Score: %.2f\n",
               records[i].name,
               records[i].age,
               records[i].score);
    }

    return 0;
}
