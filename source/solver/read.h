#ifndef READ_H
#define READ_H

void FreeMat(char **mat, int n);
char* ReadLine(FILE *file, int *n);
char **ReadFile(char *filename, int *row, int *col);

#endif
