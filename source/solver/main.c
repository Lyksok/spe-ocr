#include <stdio.h>
#include <stdlib.h>

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

void PrintWord(char *word, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%c", word[i]);
	}
	printf("\n");
}

/*
 * We expect 2 arguments (apart from the executable file)
 * the first one is the grid to read
 * the second one is the list of words to find
 * */
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		int a = argc - 1;
		printf("Expected 2 arguments, but got %i\n", a);
		return 1;
	}

	/*
	 * row : number of rows
	 * col : number of columns
	 * grid : our matrix we want to search in
	 */
	int row = 0;
	int col = 0;

	char **grid = ReadFile(argv[1], &row, &col);
	if (grid == NULL)
	{
		printf("Could not create the grid\n");
		return 3;
	}

	PrintMat(row, col, grid);

	int nblines = 0;
	Words *first = ReadWords(argv[2], &nblines);
	for (int i = 0; i < nblines; i++)
	{
		PrintWord(first->w, first->len);
		// call solver with word
		Solver(row, col, grid, first->w, first->len);
		// free word
		Words *temp = first;
		first = first->next;
		DeleteWord(temp);
	}

	FreeMat(grid, row);
	return 0;
}
