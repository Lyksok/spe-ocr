#include <stdio.h>
#include <stdlib.h>

#include "solver.h"
#include "read.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Too many arguments (%i instead of 2)\n", argc - 1);
		return 1;
	}

	/*
	 * row : pointer to the number of rows
	 * col : pointer to the number of columns
	 * grid : our matrix we want to search in
	 */
	int *row = calloc(1, sizeof(int));
	int *col = calloc(1, sizeof(int));
	if (row == NULL || col == NULL)
	{
		printf("Memory allocation failed\n");
		return 2;
	}

	char **grid = ReadFile(argv[1], row, col);
//	char grid[*row][*col] = ReadFile(argv[1], row, col);
	if (grid == NULL)
	{
		printf("Memory allocation failed\n");
		return 3;
	}

	ToUpper(argv[2]);

	int result = Solver(*row, *col, grid, argv[2]);
	if (!result)
		printf("Yeah so no it wasn't here\n");
	else
		printf("Here you go !\n");

	FreeMat(grid, *row);
	free(row);
	free(col);
	return 0;
}
