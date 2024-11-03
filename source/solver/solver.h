#ifndef SOLVER_H /* Include guard */
#define SOLVER_H

int Length(char word[]);
int Max(int a, int b);
int Min(int a, int b);
void ToUpper(char word[]);
int Search(
    //	int row, int col, char mat[row][col],
    //	int row, int col, char *mat[col],
    int row, int col, char **mat, char word[], int r, int c, int n, int *er,
    int *ec);
// int Solver(int row, int col, char mat[row][col], char word[]);
// int Solver(int row, int col, char *mat[col], char word[]);
int Solver(int row, int col, char **mat, char word[]);

#endif
