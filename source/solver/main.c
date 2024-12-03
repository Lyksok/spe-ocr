#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver.h"
#include "read.h"

void PrintMat(int row, int col, char **mat)
{
        for (int r = 0; r < row; r++)
        {
                for (int c = 0; c < col; c++)
                        printf("%c ", mat[r][c]);
                printf("\n");
        }
}

void PrintWord(char *word)
{
	int i = 0;
	while (word[i] != 0)
	{
		printf("%c", word[i]);
		i++;
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		int a = argc - 1;
		printf("Expected 2 arguments, but got %i\n", a);
		return 1;
	}

	char *file = argv[1];
	int to_print = 0;
	if (!strcmp(file, "print"))
	{
		file = argv[2];
		to_print = 1;
	}

	/*
	 * row : pointer to the number of rows
	 * col : pointer to the number of columns
	 * grid : our matrix we want to search in
	 */
	int row = 0;
	int col = 0;

	char **grid = ReadFile(file, &row, &col);
	if (grid == NULL)
	{
		printf("Could not create the grid\n");
		return 3;
	}

	if (to_print)
	{
		PrintMat(row, col, grid);
		FreeMat(grid, row);
		return 0;
	}

	/*
	 * This function checks for none letter characters
	 * It also changes the word in place to uppercase
	 */
	if (NotJustLetters(argv[2]))
	{
		FreeMat(grid, row);
		return 4;
	}

	/* FOR TESTING PURPOSE
	PrintMat(row, col, grid);
	PrintWord(argv[2]);
	*/

	int srow = 0;
	int scol = 0;
	int erow = 0;
	int ecol = 0;
	Solver(row, col, grid, argv[2], &srow, &scol, &erow, &ecol);
	// word is starting from (scol, srow) and ends at (ecol, erow)

	FreeMat(grid, row);
	return 0;
}
