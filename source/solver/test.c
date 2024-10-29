#include <stdio.h>
#include "solver.h"

void Print(int row, int col, char mat[row][col])
{
	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
			printf("%c ", mat[r][c]);
		printf("\n");
	}
}

void canIt(int row, int col, char mat[row][col], char * word)
{
	Print(row, col, mat);
        printf("Coordinates of \"%s\" be in our matrice : ", word);
        Solver(row, col, mat, word);
}

void abcd(char * word)
{
	char mat[][2] =  { {'a', 'b'}, {'c', 'd'} };
	canIt(2, 2, mat, word);
}

void abcdef(char * word)
{
	char mat[][3] =  { {'a', 'b', 'c'}, {'d', 'e', 'f'} };
	canIt(2, 3, mat, word);
}

void Hiv0(char * word)
{
	char mat[][1] = { {'H'}, {'i'} };
	canIt(2, 1, mat, word);
}

void Hiv1(char * word)
{
	char mat[][2] =  { {'H', 'i'}, {'c', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv2(char * word)
{
        char mat[][2] =  { {'H', 'o'}, {'i', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv3(char * word)
{
        char mat[][2] =  { {'o', 'i'}, {'H', 'd'} };
	canIt(2, 2, mat, word);
}

void Hiv4(char * word)
{
        char mat[][2] =  { {'H', 'o'}, {'c', 'i'} };
	canIt(2, 2, mat, word);
}

void Mat(char * word)
{
        char mat[][4] =  { 
		{'t', 'h', 'i', 's'}, 
		{'i', 's', ' ', 'a'},
       		{'t', 'e', 's', 't'}	};
        canIt(3, 4, mat, word);
}

// int main(int argc, char *argv[]) 
int main ()
{
//	abcd("nothing");
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
