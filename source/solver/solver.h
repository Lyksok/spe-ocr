#ifndef SOLVER_H /* Include guard */
#define SOLVER_H

#include <stdio.h>
#include <stdlib.h>

int Length(char word[]);
int Max(int a, int b);
int Min(int a, int b);
int NotJustLetters(char word[]);
void ToUpper(char word[]);
int Search(int row, int col, char **mat, char *word, int r, int c, int n,
           int *er, int *ec);
int Solver(int row, int col, char **mat, char *word, int len, int *sr, int *sc,
           int *er, int *ec);

#endif
