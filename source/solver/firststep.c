#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

void FreeMat(char **mat, int n)
{
	for (int i = 0; i < n; i++)
		free(*(mat + i));
	free(mat);
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
