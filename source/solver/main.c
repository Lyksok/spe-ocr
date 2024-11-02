#include <stdio.h>
#include <stdlib.h>

#include "read.h"
#include "solver.h"

void PrintMat(int row, int col, char **mat) {
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++)
      printf("%c ", mat[r][c]);
    printf("\n");
  }
}

void PrintWord(char *word) {
  int i = 0;
  while (word[i] != 0) {
    printf("%c", word[i]);
    i++;
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    int a = argc - 1;
    printf("Incorrect number of arguments (%i)\n", a);
    return 1;
  }

  /*
   * row : pointer to the number of rows
   * col : pointer to the number of columns
   * grid : our matrix we want to search in
   */
  int *row = calloc(1, sizeof(int));
  int *col = calloc(1, sizeof(int));
  if (row == NULL || col == NULL) {
    printf("Memory allocation failed (row/col)\n");
    return 2;
  }

  char **grid = ReadFile(argv[1], row, col);
  //	char grid[*row][*col] = ReadFile(argv[1], row, col);
  if (grid == NULL) {
    printf("Memory allocation failed (grid)\n");
    return 3;
  }

  ToUpper(argv[2]);

  /* FOR TESTING PURPOSE
  PrintMat(*row, *col, grid);
  PrintWord(argv[2]);
  */

  Solver(*row, *col, grid, argv[2]);
  /* FOR TESTING PURPOSE
  int result = Solver(*row, *col, grid, argv[2]);
  if (!result)
          printf("Yeah so no it wasn't here\n");
  else
          printf("Here you go !\n");
  */

  FreeMat(grid, *row);
  free(row);
  free(col);
  return 0;
}
