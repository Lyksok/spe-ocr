#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

void FreeMat(char **mat, int n)
{
	for (int i = 0; i < n; i++)
		free(*(mat + i));
	free(mat);
}

char* Readline(FILE *file) {
	/*
	 * capacity : initial buffer size
	 * length : index of the length of the line
	 * line : an array that will hold the line
	 * c : return value of fgetc()
	 */
	int capacity = 10;
	int length = 0;
	char *line = malloc(capacity * sizeof(char));
	int c;

	if (!line) {
		printf("Memory allocation failed\n");
		return NULL;
	}

	while ((c = fgetc(file)) != EOF && c != '\n') {
		// Resize the buffer if it's full
		if (length + 1 >= capacity) {
			capacity *= 2; // Double the buffer size
			char *new_line = realloc(line, capacity * sizeof(char));
			if (!new_line) {
				free(line);
				printf("Memory allocation failed\n");
				return NULL;
			}
		    line = new_line;
		}
		line[length++] = c;  // Append the character to the line
	}

	// If EOF is reached without any characters read, return NULL
	if (length == 0 && c == EOF) {
		free(line);
		return NULL;
	}

	line[length] = '\0'; // Null-terminate the line
	return line;
}

int First(char *filename, char word[])
{
        ToUpper(word);
        char *line = malloc(256 * sizeof(char));

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
                printf("Error opening file");
                return 0;
        }

        int n = 0;
        while (fgets(line, sizeof(line), file))
		n++;
        char **mat = malloc(n * sizeof(char*));

	rewind(file);

	n = 0;
        while (fgets(line, sizeof(line), file)) {
                *(mat + n) = line;
                n++;
                printf("%s", line);
        }

        fclose(file);
        free(line);

	FreeMat(mat, n);
}
