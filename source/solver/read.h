#ifndef READ_H
#define READ_H

#include <stdio.h>
typedef struct Words Words;
struct Words
{
  char *w;
  int len;
  Words *next;
};

Words *CreateWord(int len, char *word);
void LinkWords(Words *w1, Words *w2);
void DeleteWord(Words *word);

void FreeMat(char **mat, int n);
int InvalidLetter(char *line, int n);
char *ReadLine(FILE *file, int *n);
char **ReadFile(char *filename, int *row, int *col);
Words *ReadWords(char *filename, int *rows);

#endif
