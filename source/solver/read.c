#include <stdio.h>
#include <stdlib.h>

#include "read.h"
#include "solver.h"

void FreeMat(char **mat, int n) {
  for (int i = 0; i < n; i++)
    free(*(mat + i));
  free(mat);
}

/*
 * line : a list of characters
 * n : the length of said list
 * Returns whether something other than capital letters was found
 */
int InvalidLetter(char *line, int n) {
  int i = 0;
  while (i < n && line[i] >= 'A' && line[i] <= 'Z')
    i++;
  return i != n;
}

Words *CreateWord(int len, char *word) {
  Words *new = malloc(sizeof(Words));
  ;
  if (new == NULL) {
    printf("Memory allocation failed (word)\n");
    return NULL;
  }
  new->len = len;
  new->w = calloc(len, sizeof(char));
  if (new->w == NULL) {
    printf("Memory allocation failed (word)\n");
    return NULL;
  }
  for (int i = 0; i < len; i++) {
    new->w[i] = word[i];
    // printf("%c ", new->w[i]);
  }
  // printf("\n");
  new->next = NULL;
  return new;
}

void LinkWords(Words *w1, Words *w2) {
  w1->next = w2;
  return;
}

void DeleteWord(Words *word) {
  free(word->w);
  free(word);
  return;
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
   * c is an int to handle the potential errors
   *
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
    char cc = (char)c;
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
    line[length++] = cc;
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

    } else if (InvalidLetter(line, c)) {
      fclose(file);
      free(line);
      FreeMat(mat, r);
      printf("The grid should only contains");
      printf(" capital letters\n");
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

/*
 * filename : a path to a file
 * returns a linked list of Words
 * CAREFUL : need to free the list afterwards
 * CAREFUL : we do not check wether we have nothing in the file or not
 * we assume there is at least one word to read
 */
Words *ReadWords(char *filename, int *rows) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file");
    return NULL;
  }

  /*
   * line : dynamically allocated string
   * r : the number of lines in the file
   * c : the number of characters in the line
   */
  char *line;
  int r = 0;
  int c = 0;
  while ((line = ReadLine(file, &c)) != NULL) {
    r++;
    free(line);
  }
  *rows = r;

  rewind(file);

  line = ReadLine(file, &c);
  Words *first = CreateWord(c, line);
  if (first == NULL) {
    return NULL;
  }
  Words *one = first;
  free(line);
  while ((line = ReadLine(file, &c)) != NULL) {
    Words *two = CreateWord(c, line);
    if (two == NULL) {
      free(line);
      return NULL;
    }
    LinkWords(one, two);
    one = two;
    free(line);
    r++;
  }

  fclose(file);
  return first;
}
