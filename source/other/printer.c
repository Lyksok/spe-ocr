#include "printer.h"
#include <stdio.h>

void print_list_int(int **list, size_t size) {
  int *lst = *list;
  size_t i = 0;
  while (i < size) {
    printf("%i ", *(lst + i));
    if ((i + 1) % 16 == 0)
      printf("\n");
    i++;
  }
}

void print_list_double(double **list, size_t size) {
  double *lst = *list;
  size_t i = 0;
  while (i < size) {
    printf("%lf ", *(lst + i));
    if ((i + 1) % 16 == 0)
      printf("\n");
    i++;
  }
}

void print_matrix(size_t row, size_t col, char mat[row][col]) {
  for (size_t j = 0; j < row; j++) {
    for (size_t i = 0; i < col; i++) {
      printf("%c ", mat[i][j]);
    }
    printf("\n");
  }
}
