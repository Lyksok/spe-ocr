#include <stdlib.h>

#include "solver.h"
#include <stdio.h>

// void Print(int row, int col, char mat[row][col])
// void Print(int row, int col, char *mat[col])
void Print(int row, int col, char **mat) {
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++)
      printf("%c ", mat[r][c]);
    printf("\n");
  }
}

// void canIt(int row, int col, char mat[row][col], char * word)
// void canIt(int row, int col, char *mat[col], char * word)
void canIt(int row, int col, char **mat, char *word) {
  Print(row, col, mat);
  //	printf("Wow a change !\n");
  printf("Coordinates of \"%s\" be in our matrice : ", word);
  Solver(row, col, mat, word);
}

void Hiv0(char *word) {
  //	char mat[][1] = { {'H'}, {'i'} };
  char **mat = malloc(2 * sizeof(char *));
  mat[0] = "H";
  mat[1] = "i";
  canIt(2, 1, mat, word);
  free(mat);
}

void Hiv1(char *word) {
  //	char mat[][2] =  { {'H', 'i'}, {'c', 'd'} };
  char **mat = malloc(2 * sizeof(char *));
  //	char arr[2] = {'H', 'i'};
  //	*mat[0] = arr;
  //	arr = {'c', 'd'};
  //	*mat[1] = arr;
  mat[0] = "Hi";
  mat[1] = "cd";
  canIt(2, 2, mat, word);
  free(mat);
}

void Mat(char *word) {
  /*
          char mat[][4] =  {
                  {'t', 'h', 'i', 's'},
                  {'i', 's', ' ', 'a'},
                  {'t', 'e', 's', 't'}	};
  */
  char **mat = malloc(2 * sizeof(char *));
  mat[0] = "this";
  mat[1] = "is a";
  mat[2] = "test";
  canIt(3, 4, mat, word);
  free(mat);
}

// int main(int argc, char *argv[])
int main() {
  //	Hiv0("Hi");
  //	Hiv1("Hi");
  //	Hiv0("Hello");
  Mat("test");
  //	Mat("s ");
  //	Mat("");
  //	Mat("back");
  //	Mat("is");
  //	Mat("hi");
  //	Mat("she");
  //	Mat("st");
  //	Mat("at");
  //	Mat(" t");
  //	Mat("t h");
  //	Mat("tas");
  return 0;
}
