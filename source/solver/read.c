#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

void FreeMat(char **mat, int n) {
  for (int i = 0; i < n; i++)
    free(*(mat + i));
  free(mat);
}

/*
 * file : a file already open to read
 * n : value to return the number of char in the lign
 * returns the lign read
 */
char *ReadLine(FILE *file, int *n) {
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

  if (line == NULL) {
    printf("Memory allocation failed (line1)\n");
    return NULL;
  }

  while ((c = fgetc(file)) != EOF && c != '\n') {
    if (length + 1 >= capacity) {
      // it is full :
      // we add to the capacity
      capacity += 10;
      line = realloc(line, capacity * sizeof(char));
      if (line == NULL) {
        free(line);
        printf("Memory allocation failed (line2)\n");
        return NULL;
      }
    }
    // append the character to the line
    // THEN increment the length
    line[length++] = c;
  }

  // If EOF is reached without any characters read :
  // return NULL
  if (length == 0 && c == EOF) {
    free(line);
    return NULL;
  }

  line[length] = '\0';
  *n = length;
  return line;
}

/*
 * filename : a path to a file
 * row : a pointer to return the number of rows
 * col : a pointer to return the number of columns
 * returns a matrix of characters
 * CAREFUL : nothing is freed, except mat for errors
 */
char **ReadFile(char *filename, int *row, int *col) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file");
    return NULL;
  }

  /*
   * line : dynamically allocated string
   * r : the number of lines in the file
   * c : the number of characters in the line
   * mat : a matrice of n strings
   */
  char *line;
  int r = 0;
  int c = 0;
  while ((line = ReadLine(file, col)) != NULL) {
    r++;
    free(line);
  }
  char **mat = malloc(r * sizeof(char *));
  *row = r;

  //	we go back to the beginning of the file
  rewind(file);

  r = 0;
  c = *col;
  while ((line = ReadLine(file, col)) != NULL) {
    if (c != *col) {
      fclose(file);
      free(line);
      FreeMat(mat, r);
      printf("The lines do not have the same length\n");
      return NULL;
    }
    *(mat + r) = line;
    r++;
  }

  fclose(file);
  free(line);
  //	THIS IS UP TO WHO WE CALLED TO FREE
  //	FreeMat(mat, r);
  return mat;
}
